/*
 * Low-level support for LM32 remote debuging with GDB.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
 
#include <string.h>
#include <signal.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/time.h>

// #include <syscall.h>

#include "gdb.h"
#include "gdb_uart.h"

/* Enable system call support */
#undef GDB_SYSCALLS_ENABLED            1
/* Enable open/close system calls */
#undef GDB_OPEN_CLOSE_SYSCALLS_ENABLED
/* Enable support for z packets */
#undef GDB_HARDWARE_BREAKPOINTS_ENABLED
/* Enable support for q packets */
#undef GDB_ECLIPSE_SUPPORT
/* Enable support for X packets */
#define GDB_BINARY_DOWNLOAD_ENABLED     
/* Enable support for P packets */
#define GDB_P_ENABLED                   
/* Enable support for remote stub debugging */
#undef GDB_REMOTE_DEBUG_ENABLED
/* Enable support for run-length encoding */
#undef GDB_RLE_ENABLED
/* Enable support for restart packets */
#undef GDB_RESTART_ENABLED

/* Exception IDs */
#define LM32_EXCEPTION_RESET                  0x0
#define LM32_EXCEPTION_INST_BREAKPOINT        0x1
#define LM32_EXCEPTION_INST_BUS_ERROR         0x2
#define LM32_EXCEPTION_DATA_BREAKPOINT        0x3
#define LM32_EXCEPTION_DATA_BUS_ERROR         0x4
#define LM32_EXCEPTION_DIVIDE_BY_ZERO         0x5
#define LM32_EXCEPTION_INTERRUPT              0x6
#define LM32_EXCEPTION_SYSTEM_CALL            0x7

/* Breakpoint instruction */
#define LM32_BREAK                            0xac000002UL

/* BUFMAX defines the maximum number of characters in inbound/outbound buffers */
#define BUFMAX 400

/* Function prototypes */
#ifdef GDB_REMOTE_DEBUG_ENABLED
static int gdb_write (char *data, int len);
static int gdb_puts (char *str);
static void gdb_putint (int num);
#endif
static unsigned char *getpacket (void);
static unsigned char *getpacket (void);

/* For integer to ASCII conversion */
static const char hexchars[]="0123456789abcdef";

/* This numbering must be consistant with GDBs numbering in gdb/lm32-tdep.c */
enum regnames {
  R0, R1, R2, R3, R4, R5, R6, R7,
  R8, R9, R10, R11, R12, R13, R14, R15,
  R16, R17, R18, R19, R20, R21, R22, R23, 
  R24, R25, GP, FP, SP, RA, EA, BE,
  PC, EID, NUM_REGS
};

/* I/O packet buffers */
static unsigned char remcomInBuffer[BUFMAX];
static unsigned char remcomOutBuffer[BUFMAX];

/* Set by debugger to indicate that when handling memory faults (bus errors), the 
   handler should set the mem_err flag and skip over the faulting instruction */
static volatile int may_fault;

/* Set by bus error exception handler, this indicates to caller of mem2hex, 
 * hex2mem or bin2mem that there has been an error. */
static volatile int mem_err;

/* Indicates if we're single stepping */
static unsigned char stepping;
static unsigned *seq_ptr;
static unsigned seq_insn;
static unsigned *branch_ptr;                      
static unsigned branch_insn;
static char branch_step;

#ifdef GDB_REMOTE_DEBUG_ENABLED
/* debug > 0 prints ill-formed commands in valid packets & checksum errors */
static int remote_debug;
#endif

/* interrupt handler */
static void (*intr_handler)(void);

/* Convert ch from a hex digit to an int */

static int
hex (unsigned char ch)
{
  if (ch >= 'a' && ch <= 'f')
    return ch-'a'+10;
  if (ch >= '0' && ch <= '9')
    return ch-'0';
  if (ch >= 'A' && ch <= 'F')
    return ch-'A'+10;
  return -1;
}

/* Scan for the sequence $<data>#<checksum> */

static unsigned char *
getpacket (void)
{
  unsigned char *buffer = &remcomInBuffer[0];
  unsigned char checksum;
  unsigned char xmitcsum;
  int count;
  char ch;

  while (1)
    {
      /* wait around for the start character, ignore all other characters */
      while ((ch = _gdb_read_char ()) != '$')
        ;

retry:
      checksum = 0;
      xmitcsum = -1;
      count = 0;

      /* now, read until a # or end of buffer is found */
      while (count < BUFMAX)
        {
          ch = _gdb_read_char ();
          if (ch == '$')
            goto retry;
          if (ch == '#')
            break;     
          checksum = checksum + ch;
          buffer[count] = ch;
          count = count + 1;
        }
      buffer[count] = 0;

      if (ch == '#')
        {
          ch = _gdb_read_char ();
          xmitcsum = hex (ch) << 4;
          ch = _gdb_read_char ();
          xmitcsum += hex (ch);

          if (checksum != xmitcsum)
            {
#ifdef GDB_REMOTE_DEBUG_ENABLED
              if (remote_debug)
                {
                  gdb_puts ("Bad checksum: ");
                  gdb_putint (checksum);
                  gdb_puts (" != ");
                  gdb_putint (xmitcsum);
                  gdb_puts ("\n");
                }            
#endif                
              _gdb_write_char ('-');        /* failed checksum */
            }
          else
            {
              _gdb_write_char ('+');        /* successful transfer */

              /* if a sequence char is present, reply the sequence ID */
              if (buffer[2] == ':')
                {
                  _gdb_write_char (buffer[0]);
                  _gdb_write_char (buffer[1]);

                  return &buffer[3];
                }

              return &buffer[0];
            }
        }
    }
}

/* Send the packet in buffer.  */
#ifdef GDB_RLE_ENABLED
static void
putpacket (unsigned char *buffer)
{
  unsigned char checksum;
  int count;
  unsigned char ch;
  int run_length;
  int run_idx;
  char run_length_char;

  /*  $<packet info>#<checksum>. */
  do
    {
      _gdb_write_char ('$');
      checksum = 0;
      count = 0;

      while (ch = buffer[count])
        {
          /* Transmit character */
          _gdb_write_char (ch);
          checksum += ch;
          count += 1;
          /* Determine how many consecutive characters there are that are the 
           * same as the character we just transmitted */
          run_length = 0;
          run_idx = count;  
          while ((buffer[run_idx++] == ch) && (run_length < 97))
            run_length++;
          /* Encode run length as an ASCII character */
          run_length_char = (char)(run_length + 29);
          if (   (run_length >= 3) 
              && (run_length_char != '$')
              && (run_length_char != '#')
              && (run_length_char != '+')
              && (run_length_char != '-')
             )
            {
              /* Transmit run-length */ 
              _gdb_write_char ('*');
              checksum += '*';
              _gdb_write_char (run_length_char);
              checksum += run_length_char;
              count += run_length;
            }
        }
      
      _gdb_write_char ('#');
      _gdb_write_char (hexchars[(checksum >> 4) & 0xf]);
      _gdb_write_char (hexchars[checksum & 0xf]);
    }
  while (_gdb_read_char () != '+');
}
#else
static void
putpacket (unsigned char *buffer)
{
  unsigned char checksum;
  int count;
  unsigned char ch;

  /* $<packet info>#<checksum>. */
  do
    {
      _gdb_write_char ('$');
      checksum = 0;
      count = 0;

      while (ch = buffer[count])
        {
          _gdb_write_char (ch);
          checksum += ch;
          count += 1;
        }

      _gdb_write_char ('#');
      _gdb_write_char (hexchars[checksum >> 4]);
      _gdb_write_char (hexchars[checksum % 16]);
    }
  while (_gdb_read_char () != '+');
}
#endif

#ifdef GDB_REMOTE_DEBUG_ENABLED

/* Make gdb write n bytes to stdout (not assumed to be null-terminated).
   Returns: number of bytes written */

static int
gdb_write (char *data, int len)
{
  char *buf, *cpy;
  int i;
  char temp[100];

  buf = temp;
  buf[0] = 'O';
  i = 0;
  while (i < len)
    {
      for (cpy = buf + 1;
	   i < len && cpy < buf + sizeof (temp) - 3; i++)
	{
	  *cpy++ = hexchars[data[i] >> 4];
	  *cpy++ = hexchars[data[i] & 0x0F];
	}
      *cpy = 0;
      putpacket (buf);
    }
  return len;
}

/* Make gdb write a null-terminated string to stdout.
   Returns: the length of the string */
   
static int 
gdb_puts (char *str)
{
  return gdb_write (str, strlen (str));
}

/* Make gdb write an integer to stdout. */
   
static void 
gdb_putint (int num)
{
  char  buf[9];
  int   cnt;
  char  *ptr;
  int   digit;
  
  ptr = buf;
  for (cnt = 7 ; cnt >= 0 ; cnt--) {
    digit = (num >> (cnt * 4)) & 0xf;
    
    if (digit <= 9)
      *ptr++ = (char) ('0' + digit);
    else
      *ptr++ = (char) ('a' - 10 + digit);
  }

  *ptr = (char) 0;
  gdb_puts (buf);
}

#endif

static void
allow_nested_exception ()
{
  mem_err = 0;
  may_fault = 1;
}

static void
disallow_nested_exception ()
{
  mem_err = 0;
  may_fault = 0;
}

/* Convert the memory pointed to by mem into hex, placing result in buf.
 * Return a pointer to the last char put in buf ('\0'), in case of mem fault,
 * return NULL.
 */

static unsigned char *
mem2hex (unsigned char *mem, unsigned char *buf, int count)
{
  unsigned char ch;
           
#if 0
  /* Some h/w registers require word/half-word access, so treat them as a special case */           
  if ((count == 4) && (((unsigned)mem & 3) == 0))
    {
      unsigned long val;
      int i;
      
      /* Read 32-bit value from memory */
      val = *(unsigned long *)mem;
      /* Return NULL if the memory access caused an exception */
      if (mem_err)
        return NULL;
      /* Convert 32-bit value to a hex string */
      for (i = 28; i >= 0; i -= 4) 
        *buf++ = hexchars[(val >> i) & 0xf];
    }
  else if ((count == 2) && (((unsigned)mem & 1) == 0))
    {
      unsigned short val;
      int i;
      
      /* Read 16-bit value from memory */
      val = *(unsigned short *)mem;
      /* Return NULL if the memory access caused an exception */
      if (mem_err)
        return NULL;
      /* Convert 16-bit value to a hex string */
      for (i = 12; i >= 0; i -= 4) 
        *buf++ = hexchars[(val >> i) & 0xf];        
    }
  else 
#endif 
    {           
      while (count-- > 0)
        {
          ch = *mem++;
          if (mem_err)
            return NULL;
          *buf++ = hexchars[(ch >> 4) & 0xf];
          *buf++ = hexchars[ch & 0xf];
        }
    }

  *buf = '\0';
  return buf;
}

/* convert the hex array pointed to by buf into binary to be placed in mem
 * return a pointer to the character AFTER the last byte written */

static char *
hex2mem (unsigned char *buf, unsigned char *mem, int count)
{
  int i;
  unsigned char ch;

#if 0
  /* Some h/w registers require word/half-word access, so treat them as a special case */           
  if ((count == 4) && (((unsigned)mem & 3) == 0))
    {
      unsigned long val;
      int i;

      /* Convert hex data to 32-bit value */
      val = 0;
      for (i = 24; i >= 0; i -= 4) 
        val |= hex (*buf++) << i;
      /* Attempt to write data to memory */
      *(unsigned long *)mem = val;
      /* Return NULL if write caused an exception */      
      if (mem_err)
        return NULL;
      mem += 4;
    }
  else if ((count == 2) && (((unsigned)mem & 1) == 0))
    {
      unsigned short val;
      
      /* Convert hex data to 16-bit value */
      val = 0;
      for (i = 12; i >= 0; i -= 4) 
        val |= hex (*buf++) << i;
      /* Attempt to write data to memory */
      *(unsigned short *)mem = val;
      /* Return NULL if write caused an exception */      
      if (mem_err)
        return NULL;
      mem += 2;
    }
  else 
#endif  
    {           
      for (i = 0; i < count; i++)
        {
          /* Convert hex data to 8-bit value */
          ch = hex (*buf++) << 4;
          ch |= hex (*buf++);
          /* Attempt to write data to memory */
          *mem++ = ch;
          /* Return NULL if write caused an exception */      
          if (mem_err)
            return NULL;
        }
    }
  return mem;
}

#ifdef GDB_BINARY_DOWNLOAD_ENABLED

/* Copy the binary data pointed to by buf to mem and
 * return a pointer to the character AFTER the last byte written 
 * $, # and 0x7d are escaped with 0x7d */ 

static char *
bin2mem (unsigned char *buf, unsigned char *mem, int count)
{
  int i;
  unsigned char c;

#if 0
  /* Some h/w registers require word/half-word access, so treat them as a special case */           
  if ((count == 4) && (((unsigned)mem & 3) == 0))
    {
      unsigned long val;
      int i;

      /* Convert binary data to 32-bit value */
      val = 0;
      for (i = 24; i >= 0; i -= 8) 
        {
          c = *buf++;
          if (c == 0x7d)
            c = *buf++ ^ 0x20;  
          val |= c << i;
        }          
      /* Attempt to write value to memory */  
      *(unsigned long *)mem = val;
      /* Return NULL if write caused an exception */      
      if (mem_err)
        return NULL;
      mem += 4;
    }
  else if ((count == 2) && (((unsigned)mem & 1) == 0))
    {
      unsigned short val;
      int i;
      
      /* Convert binary data to 16-bit */
      val = 0;
      for (i = 8; i >= 0; i -= 8) 
        {
          c = *buf++;
          if (c == 0x7d)
            c = *buf++ ^ 0x20;  
          val |= c << i;
        }          
      /* Attempt to write value to memory */  
      *(unsigned short *)mem = val;
      /* Return NULL if write caused an exception */      
      if (mem_err)
        return NULL;
      mem += 2;
    }
  else 
#endif
    {             
      for (i = 0; i < count; i++)
        {
          /* Convert binary data to unsigned byte */
          c = *buf++;
          if (c == 0x7d)
            c = *buf++ ^ 0x20;
          /* Attempt to write value to memory */  
          *mem++ = c;
          /* Return NULL if write caused an exception */      
          if (mem_err)
            return NULL; 
        }
    }
  return mem;
}
#endif

/* Convert the exception identifier to a signal number. */

static int
computeSignal (int eid)
{
  switch (eid)
    {
    case LM32_EXCEPTION_RESET:
      return 0;
    case LM32_EXCEPTION_INTERRUPT:
      return SIGINT;
    case LM32_EXCEPTION_DATA_BREAKPOINT:
    case LM32_EXCEPTION_INST_BREAKPOINT:
      return SIGTRAP;
    case LM32_EXCEPTION_INST_BUS_ERROR:
    case LM32_EXCEPTION_DATA_BUS_ERROR:
      return SIGSEGV;
    case LM32_EXCEPTION_DIVIDE_BY_ZERO:
      return SIGFPE;
    }  
  return SIGHUP;  /* default for things we don't know about */
}

/* Flush the instruction cache */

static void
flush_i_cache (void)
{
  /* Executing this does no harm on CPUs without a cache */
  /* We flush DCache as well incase debugger has accessed memory directly */
  __asm__ __volatile__ ("wcsr ICC, %0\n"
                        "nop\n"
                        "nop\n"
                        "nop\n" 
                        "wcsr DCC, %0\n"
                        "nop\n"
                        "nop\n"
                        "nop" 
                        : 
                        : "r" (1)
                       );
}

/*
 * While we find nice hex chars, build an int.
 * Return number of chars processed.
 */

static int
hexToInt (char **ptr, int *intValue)
{
  int numChars = 0;
  int hexValue;

  *intValue = 0;

  while (**ptr)
    {
      hexValue = hex(**ptr);
      if (hexValue < 0)
        break;

      *intValue = (*intValue << 4) | hexValue;
      numChars ++;

      (*ptr)++;
    }

  return (numChars);
}

/* Convert a register to a hex string */

static unsigned char *
reg2hex (unsigned val, unsigned char *buf)
{
  *buf++ = hexchars[(val >> 28) & 0xf];
  *buf++ = hexchars[(val >> 24) & 0xf];
  *buf++ = hexchars[(val >> 20) & 0xf];
  *buf++ = hexchars[(val >> 16) & 0xf];
  *buf++ = hexchars[(val >> 12) & 0xf];
  *buf++ = hexchars[(val >> 8) & 0xf];
  *buf++ = hexchars[(val >> 4) & 0xf];
  *buf++ = hexchars[val & 0xf];

  return buf;
}

#ifdef GDB_HARDWARE_BREAKPOINTS_ENABLED

/* Set a h/w breakpoint at the given address */

static int
set_hw_breakpoint(int address, int length)
{
  int bp;
  
  /* Find a free break point register and then set it */      
  __asm__ ("rcsr  %0, BP0" : "=d" (bp));
  if ((bp & 0x01) == 0) 
    {
      __asm__ ("wcsr  BP0, %0" : : "d" (address | 1));
      return 1;
    }
  __asm__ ("rcsr  %0, BP1" : "=d" (bp));
  if ((bp & 0x01) == 0) 
    {
      __asm__ ("wcsr  BP1, %0" : : "d" (address | 1));
      return 1;
    }
  __asm__ ("rcsr  %0, BP2" : "=d" (bp));
  if ((bp & 0x01) == 0) 
    {
      __asm__ ("wcsr  BP2, %0" : : "d" (address | 1));
      return 1;
    }
  __asm__ ("rcsr  %0, BP3" : "=d" (bp));
  if ((bp & 0x01) == 0) 
    {
      __asm__ ("wcsr  BP3, %0" : : "d" (address | 1));
      return 1;
    }
  /* No free breakpoint registers */
  return -1;        
}

/* Remove a h/w breakpoint which should be set at the given address */

static int 
disable_hw_breakpoint(int address, int length)
{
  int bp;
  
  /* Try to find matching breakpoint register */
  __asm__ ("rcsr  %0, BP0" : "=d" (bp));
  if ((bp & 0xfffffffc) == (address & 0xfffffffc)) 
    {
      __asm__ ("wcsr  BP0, %0" : : "d" (0));
      return 1;
    }
  __asm__ ("rcsr  %0, BP1" : "=d" (bp));
  if ((bp & 0xfffffffc) == (address & 0xfffffffc)) 
    {
      __asm__ ("wcsr  BP1, %0" : : "d" (0));
      return 1;
    }
  __asm__ ("rcsr  %0, BP2" : "=d" (bp));
  if ((bp & 0xfffffffc) == (address & 0xfffffffc)) 
    {
      __asm__ ("wcsr  BP2, %0" : : "d" (0));
      return 1;
    }
  __asm__ ("rcsr  %0, BP3" : "=d" (bp));
  if ((bp & 0xfffffffc) == (address & 0xfffffffc)) 
    {
      __asm__ ("wcsr  BP3, %0" : : "d" (0));
      return 1;
    }
  /* Breakpoint not found */
  return -1;
}

#endif

/* This function does all command procesing for interfacing to gdb. 
 * The error codes we return are errno numbers */

void
_handle_exception (unsigned int *registers)
{
  int tt;                        /* Trap type */
  int sigval;
  int addr;
  int length;
  char *ptr;
  unsigned int *sp;
  int err;
  unsigned int dc;     
  int pathlen;                       
  int retcode;
  int reterrno;
  int reg;
  unsigned char status;
  unsigned insn;
  unsigned opcode;
  unsigned branch_target;
  
  /* Check for bus error caused by this code (rather than the program being debugged) */
  if (may_fault && (registers[EID] == LM32_EXCEPTION_DATA_BUS_ERROR))
    {
#ifdef GDB_REMOTE_DEBUG_ENABLED
      if (remote_debug)
        gdb_puts ("Bus error in monitor\n");
#endif        
      /* Indicate that a fault occured */
      mem_err = 1;
      /* Skip over faulting instruction */
      registers[PC] += 4;      
      /* Resume execution */
      return;
    }

  if (stepping)
    {
      /* Remove breakpoints */
      *seq_ptr = seq_insn;
      if (branch_step)      
        *branch_ptr = branch_insn;      
      stepping = 0;
    }   
            
  /* Convert exception ID to a signal number */          
  sigval = computeSignal(registers[EID]);  
  if (sigval == SIGINT) {
    if (intr_handler != NULL) {
	(*intr_handler)();
	return;
    } else {
    	_gdb_ack_interrupt ();
    }
  }
    
  /* Set pointer to start of output buffer */  
  ptr = remcomOutBuffer;
  
#ifdef GDB_SYSCALLS_ENABLED
  if (registers[EID] == LM32_EXCEPTION_SYSTEM_CALL)
    {        
      /*_gpio.OutData = 0x82;*/

      /* Calls to strlen in the following code may cause bus errors */
      allow_nested_exception ();
      /* Pass system calls to the debugger */        
      switch (registers[R8])
        {
          case SYS_exit:
            *ptr++ = 'W';
            *ptr++ = hexchars[(registers[R1] >> 4) & 0xf];
            *ptr++ = hexchars[registers[R1] & 0xf];
            *ptr++ = 0;               
            intr_handler = NULL;
            break;
            
#ifdef GDB_OPEN_CLOSE_SYSCALLS_ENABLED
          case SYS_open:
            memcpy (ptr, "Fopen,", 6);
            ptr += 6;
            ptr = reg2hex(registers[R1], ptr);
            *ptr++ = '/';
            pathlen = strlen((unsigned char *)registers[R1]) + 1;
            ptr = reg2hex(pathlen, ptr);
            *ptr++ = ',';
            ptr = reg2hex(registers[R2], ptr);
            *ptr++ = ',';
            ptr = reg2hex(registers[R3], ptr);
            *ptr++ = 0;               
            break;
                      
          case SYS_close:
            memcpy (ptr, "Fclose,", 7);
            ptr += 7;
            ptr = reg2hex(registers[R1], ptr);
            *ptr++ = 0;               
            break;
#endif            
                      
          case SYS_read:
            memcpy (ptr, "Fread,", 6);
            ptr += 6;
            ptr = reg2hex(registers[R1], ptr);
            *ptr++ = ',';
            ptr = reg2hex(registers[R2], ptr);
            *ptr++ = ',';
            ptr = reg2hex(registers[R3], ptr);
            *ptr++ = 0;               
            break;
            
          case SYS_write:
            memcpy (ptr, "Fwrite,", 7);
            ptr += 7;
            ptr = reg2hex(registers[R1], ptr);
            *ptr++ = ',';
            ptr = reg2hex(registers[R2], ptr);
            *ptr++ = ',';
            ptr = reg2hex(registers[R3], ptr);
            *ptr++ = 0;               
            break;                        
               
          case 231:	    
            /*_gpio.OutData = 0x90;*/
            intr_handler = registers[R1]; 
            /* Skip over instruction */
            registers[PC] += 4;	    
            return;

          case 232:	    
            intr_handler = NULL;
            /* Skip over instruction */
            registers[PC] += 4;	    
            return;

          default:          /* Unknown or unsupported system call */
            /* Indicate to calling program that its not supported */
            registers[R1] = -1;
            registers[R2] = 0;
            registers[R3] = ENOSYS;
            /* Skip over instruction */
            registers[PC] += 4;      
            return;
        } 
      /* Check to see if a bus error occured */
      if (mem_err)
        {          
          disallow_nested_exception ();
          /* Indicate error to calling program */
          registers[R1] = -1;
          registers[R2] = 0;
          registers[R3] = ENOSYS;
          /* Skip over scall instruction */
          registers[PC] += 4; 
          return;     
        } 
      disallow_nested_exception ();
    }
  else 
#endif /* GDB_SYSCALLS_ENABLED */  
    {
      /* reply to host that an exception has occurred */
    
      *ptr++ = 'T';
      
      *ptr++ = hexchars[(sigval >> 4) & 0xf];
      *ptr++ = hexchars[sigval & 0xf];
    
      *ptr++ = hexchars[(PC >> 4) & 0xf];
      *ptr++ = hexchars[PC & 0xf];
      *ptr++ = ':';
      ptr = mem2hex ((unsigned char *)&registers[PC], ptr, 4);
      *ptr++ = ';';
    
      *ptr++ = hexchars[(SP >> 4) & 0xf];
      *ptr++ = hexchars[SP & 0xf];
      *ptr++ = ':';
      ptr = mem2hex ((unsigned char *)&registers[SP], ptr, 4);
      *ptr++ = ';';
      
      *ptr++ = 0;               
    }    
  
  if (registers[EID])
    putpacket (remcomOutBuffer);

  while (1)
    {
      remcomOutBuffer[0] = 0;

      ptr = getpacket();
      
      switch (*ptr++)
        {
        case '?':               /* return last signal */
          remcomOutBuffer[0] = 'S';
          remcomOutBuffer[1] = hexchars[sigval >> 4];
          remcomOutBuffer[2] = hexchars[sigval & 0xf];
          remcomOutBuffer[3] = 0;
          break;

#ifdef GDB_REMOTE_DEBUG_ENABLED 
        case 'd':                /* toggle debug flag */
          remote_debug = !(remote_debug);	
          break;
#endif          

        case 'g':                /* return the value of the CPU registers */
          ptr = remcomOutBuffer;
          ptr = mem2hex ((unsigned char *)registers, ptr, NUM_REGS * 4); 
          break;

        case 'G':               /* set the value of the CPU registers */
          hex2mem (ptr, (unsigned char *)registers, NUM_REGS * 4); 
          strcpy (remcomOutBuffer, "OK");
          break;

#ifdef GDB_P_ENABLED
        case 'p':               /* Return the value of the specified register */          
          if (hexToInt (&ptr, &reg))
            {
              ptr = remcomOutBuffer;
              ptr = mem2hex ((unsigned char *)&registers[reg], ptr, 4);
            }
          else 
            strcpy (remcomOutBuffer, "E22");
          break;

        case 'P':               /* Set the specified register to the given value */
          if (hexToInt (&ptr, &reg)
              && *ptr++ == '=')
            {
              hex2mem (ptr, (unsigned char *)&registers[reg], 4);
              strcpy (remcomOutBuffer, "OK");
            }
          else
            strcpy (remcomOutBuffer, "E22");
          break;
#endif          

        case 'm':               /* mAA..AA,LLLL  Read LLLL bytes at address AA..AA */
          /* Try to read %x,%x.  */
          if (hexToInt (&ptr, &addr)
              && *ptr++ == ','
              && hexToInt (&ptr, &length)
              && length < (sizeof(remcomOutBuffer)/2))
            {
              allow_nested_exception ();
              if (NULL == mem2hex((unsigned char *)addr, remcomOutBuffer, length))
                strcpy (remcomOutBuffer, "E14");
              disallow_nested_exception ();
            }
          else
            strcpy (remcomOutBuffer,"E22");
          break;

        case 'M': /* MAA.AA,LLLL: Write LLLL bytes at address AA.AA */
          /* Try to read '%x,%x:'.  */
          if (hexToInt (&ptr, &addr)
              && *ptr++ == ','
              && hexToInt (&ptr, &length)
              && *ptr++ == ':')
            {
              allow_nested_exception ();
              if (hex2mem(ptr, (char *)addr, length))
                strcpy (remcomOutBuffer, "OK");
              else
                strcpy (remcomOutBuffer, "E14");
              disallow_nested_exception ();
            }
          else
            strcpy (remcomOutBuffer, "E22");
          break;

#ifdef GDB_BINARY_DOWNLOAD_ENABLED
        case 'X': /* XAA.AA,LLLL: Write LLLL bytes at address AA.AA */
          /* Try to read '%x,%x:'.  */
          if (hexToInt (&ptr, &addr)
              && *ptr++ == ','
              && hexToInt (&ptr, &length)
              && *ptr++ == ':')
            {
              allow_nested_exception ();
              if (bin2mem (ptr, (unsigned char *)addr, length))
                strcpy (remcomOutBuffer, "OK");
              else
                strcpy (remcomOutBuffer, "E14");
              disallow_nested_exception ();
            }
          else
            strcpy (remcomOutBuffer, "E22");
          break;
#endif          

#if 0
        case 'C':    /* CSS;AA..AA    Continue with signal SS at address AA..AA(optional) */
          /* Set signal number */
          if (hexToInt (&ptr, &sigval))
            registers[EID] = sigval;
          /* try to read optional parameter, pc unchanged if no parm */
          if (*ptr == ';')
            {
              ptr++;
              if (hexToInt (&ptr, &addr))
                registers[PC] = addr;
            }
          flush_i_cache ();
          return;
#endif          

        case 'c':    /* cAA..AA    Continue at address AA..AA(optional) */
          /* try to read optional parameter, pc unchanged if no parm */
          if (hexToInt (&ptr, &addr))
            registers[PC] = addr;
          flush_i_cache ();
          return;
          
        case 's':               /* step at address AA (optional) */
          /* try to read optional parameter, pc unchanged if no parm */
          if (hexToInt (&ptr, &addr))
            registers[PC] = addr;
          stepping = 1;
          /* Is instruction a branch? */
          insn = *(unsigned *)registers[PC];
          opcode = insn & 0xfc000000;
          if (   (opcode == 0xe0000000)
              || (opcode == 0xf8000000)
             ) 
            {
              branch_step = 1;
              branch_target = registers[PC] + (((signed)insn << 6) >> 4);
            }   
          else if (   (opcode == 0x44000000)
                   || (opcode == 0x48000000) 
                   || (opcode == 0x4c000000) 
                   || (opcode == 0x50000000) 
                   || (opcode == 0x54000000) 
                   || (opcode == 0x5c000000) 
                  ) 
            {
             branch_step = 1;
             branch_target = registers[PC] + (((signed)insn << 16) >> 14);
            }
          else if (   (opcode == 0xd8000000)
                   || (opcode == 0xc0000000) 
                  )
            {
              branch_step = 1;
              branch_target = registers[(insn >> 21) & 0x1f];
            }      
          else 
            branch_step = 0;
          /* Set breakpoint after instruction we're stepping */
          seq_ptr = (unsigned *)registers[PC];
          seq_ptr++;
          seq_insn = *seq_ptr;
          *seq_ptr = LM32_BREAK;
          if (branch_step)
            {
              /* Set breakpoint on branch target */
              branch_ptr = (unsigned *)branch_target;
              branch_insn = *branch_ptr;
              *branch_ptr = LM32_BREAK;
            }
          flush_i_cache ();
          return;

#ifdef GDB_HARDWARE_BREAKPOINTS_ENABLED
        case 'Z': 
          switch (*ptr++) 
            {
            case '1':           /* Insert h/w breakpoint */
              if (*ptr++ == ','
                  && hexToInt (&ptr, &addr)
                  && *ptr++ == ','
                  && hexToInt (&ptr, &length))
                {
                  err = set_hw_breakpoint(addr, length);
                  if (err > 0)
                    strcpy (remcomOutBuffer, "OK");
                  else if (err < 0)
                    strcpy (remcomOutBuffer, "E28"); 
                }
              else
                strcpy (remcomOutBuffer, "E22");
              break;              
            }
          break;
                    
        case 'z':
          switch (*ptr++)
            {
            case '1':           /* Remove h/w breakpoint */
              if (*ptr++ == ','
                  && hexToInt (&ptr, &addr)
                  && *ptr++ == ','
                  && hexToInt (&ptr, &length))
                {
                  err = disable_hw_breakpoint(addr, length);
                  if (err > 0)
                    strcpy (remcomOutBuffer, "OK");
                  else if (err < 0)
                    strcpy (remcomOutBuffer, "E28"); 
                }              
              else
                strcpy (remcomOutBuffer, "E22");
              break; 
            }
          break;
#endif          
                     
#ifdef GDB_SYSCALLS_ENABLED
        case 'F':               /* system call result */  
          if (   (ptr[0] == '-') 
              && (ptr[1] == '1')
              && (ptr[2] == ',')
             ) 
            {
              /* System call failed */
              ptr += 3;
              hexToInt (&ptr, &reterrno);
              retcode = -1;
            }
          else
            {
              /* System call was successful */
              hexToInt (&ptr, &retcode);
              allow_nested_exception ();
              /* Check if a bus error occured when mapping data structures */  
              if (mem_err)
                {
                  reterrno = EFAULT;                       
                  retcode = -1;
                }
              disallow_nested_exception ();
            }
          /* Skip over scall instruction */
          registers[PC] += 4;
          /* Set return value */
          registers[R1] = retcode;
          registers[R2] = 0;
          registers[R3] = reterrno;
          return;
#endif /* GDB_SYSCALLS_ENABLED */          
        
#ifdef GDB_ECLIPSE_SUPPORT        
        case 'q':               /* Query */
          if (ptr[0] == 'C')
            {
              /* Return current thread ID. We only support 1. */
              strcpy (remcomOutBuffer, "qC1");
            }
          else if (!strncmp (&ptr[0], "fThreadInfo", 11))
            {                                      
              /* Return all thread IDs. We only support 1. */
              strcpy (remcomOutBuffer, "m1");
            }
          else if (!strncmp (&ptr[0], "sThreadInfo", 11))
            {
              /* Indicate there are no more threads. */
              strcpy (remcomOutBuffer, "l");
            }
          break;
#endif          

#ifdef GDB_RESTART_ENABLED      
        case 'r':               /* Reset */
        case 'R':
          /* We reset by branching to the reset exception handler. */
          registers[PC] = 0;
          return;
#endif          
          
        }                       

      /* reply to the request */
      putpacket (remcomOutBuffer);
    }
}


