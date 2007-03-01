--************************************************************************************************
--  Top entity for AVR core
--  Version 1.11
--  Designed by Ruslan Lepetenok 
--  Modified 03.11.2002
--************************************************************************************************

library IEEE;
use IEEE.std_logic_1164.all;

entity avr_core is port
(

cp2      : in std_logic;
ireset   : in std_logic;
cpuwait  : in std_logic;

-- PROGRAM MEMORY PORTS
pc       : out std_logic_vector (15 downto 0);   -- CORE OUTPUT	  !CHECKED!
inst     : in std_logic_vector (15 downto 0);	-- CORE INPUT     !CHECKED!

-- I/O REGISTERS PORTS
adr      : out std_logic_vector (5 downto 0); 	-- CORE OUTPUT  ????
iore     : out std_logic;                       -- CORE OUTPUT  !CHECKED!
iowe     : out std_logic;						-- CORE OUTPUT  !CHECKED!

-- DATA MEMORY PORTS
ramadr   : out std_logic_vector (15 downto 0);
ramre    : out std_logic;
ramwe    : out std_logic;

dbusin   : in std_logic_vector (7 downto 0);
dbusout  : out std_logic_vector (7 downto 0);

-- INTERRUPTS PORT
irqlines : in std_logic_vector (22 downto 0);
irqack   : out std_logic;
irqackad : out std_logic_vector(4 downto 0)

);

end avr_core;

architecture struct of avr_core is

-- *****************************************************************************************
component pm_fetch_dec is port
(

-- EXTERNAL INTERFACES OF THE CORE

clk     : in std_logic;
nrst    : in std_logic;
cpuwait : in std_logic;

-- PROGRAM MEMORY PORTS
pc      : out std_logic_vector (15 downto 0);   -- CORE OUTPUT	  !CHECKED!
inst    : in std_logic_vector (15 downto 0);	-- CORE INPUT     !CHECKED!

-- I/O REGISTERS PORTS
adr      : out std_logic_vector (5 downto 0); 	-- CORE OUTPUT  ????
iore     : out std_logic;                       -- CORE OUTPUT  !CHECKED!
iowe     : out std_logic;						-- CORE OUTPUT  !CHECKED!

-- DATA MEMORY PORTS
ramadr   : out std_logic_vector (15 downto 0);
ramre    : out std_logic;
ramwe    : out std_logic;

dbusin   : in std_logic_vector (7 downto 0);
dbusout  : out std_logic_vector (7 downto 0);

-- INTERRUPTS PORT
irqlines : in std_logic_vector (22 downto 0);
irqack   : out std_logic;
irqackad : out std_logic_vector(4 downto 0);

-- END OF THE CORE INTERFACES


-- *********************************************************************************************
-- ******************** INTERFACES TO THE OTHER BLOCKS *****************************************
-- *********************************************************************************************


-- *********************************************************************************************
-- ******************** INTERFACES TO THE ALU  *************************************************
-- *********************************************************************************************
              alu_data_r_in       : out std_logic_vector(7 downto 0);
              alu_data_d_in       : out std_logic_vector(7 downto 0);

-- OPERATION SIGNALS INPUTS

              idc_add_out         : out std_logic;
              idc_adc_out         : out std_logic;
              idc_adiw_out        : out std_logic;
              idc_sub_out         : out std_logic;
              idc_subi_out        : out std_logic;
              idc_sbc_out         : out std_logic;
              idc_sbci_out        : out std_logic;
              idc_sbiw_out        : out std_logic;

              adiw_st_out         : out std_logic;
              sbiw_st_out         : out std_logic;

              idc_and_out         : out std_logic;
              idc_andi_out        : out std_logic;
              idc_or_out          : out std_logic;
              idc_ori_out         : out std_logic;
              idc_eor_out         : out std_logic;              
              idc_com_out         : out std_logic;              
              idc_neg_out         : out std_logic;

              idc_inc_out         : out std_logic;
              idc_dec_out         : out std_logic;

              idc_cp_out          : out std_logic;              
              idc_cpc_out         : out std_logic;
              idc_cpi_out         : out std_logic;
              idc_cpse_out        : out std_logic;                            

              idc_lsr_out         : out std_logic;
              idc_ror_out         : out std_logic;
              idc_asr_out         : out std_logic;
              idc_swap_out        : out std_logic;


-- DATA OUTPUT
              alu_data_out        : in std_logic_vector(7 downto 0);

-- FLAGS OUTPUT
              alu_c_flag_out      : in std_logic;
              alu_z_flag_out      : in std_logic;
              alu_n_flag_out      : in std_logic;
              alu_v_flag_out      : in std_logic;
              alu_s_flag_out      : in std_logic;
              alu_h_flag_out      : in std_logic;

-- *********************************************************************************************
-- ******************** INTERFACES TO THE GENERAL PURPOSE REGISTER FILE ************************
-- *********************************************************************************************
          reg_rd_in   : out std_logic_vector  (7 downto 0);
          reg_rd_out  : in  std_logic_vector  (7 downto 0);
          reg_rd_adr  : out std_logic_vector  (4 downto 0);
          reg_rr_out  : in  std_logic_vector  (7 downto 0);
          reg_rr_adr  : out std_logic_vector  (4 downto 0);
          reg_rd_wr   : out std_logic;

          post_inc    : out std_logic;                       -- POST INCREMENT FOR LD/ST INSTRUCTIONS
          pre_dec     : out std_logic;                        -- PRE DECREMENT FOR LD/ST INSTRUCTIONS
          reg_h_wr    : out std_logic;
          reg_h_out   : in  std_logic_vector (15 downto 0);
          reg_h_adr   : out std_logic_vector (2 downto 0);    -- x,y,z
   		  reg_z_out   : in  std_logic_vector (15 downto 0);  -- OUTPUT OF R31:R30 FOR LPM/ELPM/IJMP INSTRUCTIONS

-- *********************************************************************************************
-- ******************** INTERFACES TO THE INPUT/OUTPUT REGISTER FILE ***************************
-- *********************************************************************************************
--          adr          : out std_logic_vector(5 downto 0);         
--          iowe         : out std_logic;         
          
--        dbusout      : out std_logic_vector(7 downto 0);  -- OUTPUT OF THE CORE

          sreg_fl_in    : out std_logic_vector(7 downto 0); -- ????        
          sreg_out      : in  std_logic_vector(7 downto 0);  -- ????       

          sreg_fl_wr_en : out std_logic_vector(7 downto 0);   --FLAGS WRITE ENABLE SIGNALS       

          spl_out       : in  std_logic_vector(7 downto 0);         
          sph_out       : in  std_logic_vector(7 downto 0);         
          sp_ndown_up   : out std_logic; -- DIRECTION OF CHANGING OF STACK POINTER SPH:SPL 0->UP(+) 1->DOWN(-)
          sp_en         : out std_logic; -- WRITE ENABLE(COUNT ENABLE) FOR SPH AND SPL REGISTERS
  
          rampz_out     : in std_logic_vector(7 downto 0);

-- *********************************************************************************************
-- ******************** INTERFACES TO THE INPUT/OUTPUT ADDRESS DECODER  ************************
-- *********************************************************************************************
          
--		  ram_data_in  : in std_logic_vector (7 downto 0);
--          adr          : in std_logic_vector(5 downto 0);         
--          iore         : in std_logic;       -- CORE SIGNAL         
--          ramre        : in std_logic;	   -- CORE SIGNAL         
--          dbusin       : out std_logic_vector(7 downto 0));	-- CORE SIGNAL         			 

-- *********************************************************************************************
-- ******************** INTERFACES TO THE BIT PROCESSOR   **************************************
-- *********************************************************************************************

              bit_num_r_io    : out std_logic_vector (2 downto 0);   -- BIT NUMBER FOR CBI/SBI/BLD/BST/SBRS/SBRC/SBIC/SBIS INSTRUCTIONS
--              dbusin          : in  std_logic_vector(7 downto 0);  -- SBI/CBI/SBIS/SBIC  IN
              bitpr_io_out    : in std_logic_vector(7 downto 0);     -- SBI/CBI OUT        

              branch     : out  std_logic_vector (2 downto 0);  -- NUMBER (0..7) OF BRANCH CONDITION FOR BRBS/BRBC INSTRUCTION

              bit_pr_sreg_out : in std_logic_vector(7 downto 0);     -- BCLR/BSET/BST(T-FLAG ONLY)             

              sreg_bit_num    : out std_logic_vector(2 downto 0);    -- BIT NUMBER FOR BCLR/BSET INSTRUCTIONS

              bld_op_out      : in std_logic_vector(7 downto 0);     -- BLD OUT (T FLAG)

              bit_test_op_out : in std_logic;                        -- OUTPUT OF SBIC/SBIS/SBRS/SBRC


-- OPERATION SIGNALS INPUTS

              -- INSTRUCTUIONS AND STATES

              idc_sbi_out     : out std_logic;
              sbi_st_out      : out std_logic;
              idc_cbi_out     : out std_logic;
              cbi_st_out      : out std_logic;

              idc_bld_out     : out std_logic;
              idc_bst_out     : out std_logic;
              idc_bset_out    : out std_logic;
              idc_bclr_out    : out std_logic;

              idc_sbic_out    : out std_logic;
              idc_sbis_out    : out std_logic;
              
              idc_sbrs_out    : out std_logic;
              idc_sbrc_out    : out std_logic;
              
              idc_brbs_out    : out std_logic;
              idc_brbc_out    : out std_logic;

              idc_reti_out    : out std_logic

-- *********************************************************************************************
-- ******************** END OF INTERFACES TO THE OTHER BLOCKS  *********************************
-- *********************************************************************************************
			  
				 
);

end component;


component alu_avr is port(

              alu_data_r_in   : in std_logic_vector(7 downto 0);
              alu_data_d_in   : in std_logic_vector(7 downto 0);
              
              alu_c_flag_in   : in std_logic;
              alu_z_flag_in   : in std_logic;


-- OPERATION SIGNALS INPUTS
              idc_add         :in std_logic;
              idc_adc         :in std_logic;
              idc_adiw        :in std_logic;
              idc_sub         :in std_logic;
              idc_subi        :in std_logic;
              idc_sbc         :in std_logic;
              idc_sbci        :in std_logic;
              idc_sbiw        :in std_logic;

              adiw_st         : in std_logic;
              sbiw_st         : in std_logic;

              idc_and         :in std_logic;
              idc_andi        :in std_logic;
              idc_or          :in std_logic;
              idc_ori         :in std_logic;
              idc_eor         :in std_logic;              
              idc_com         :in std_logic;              
              idc_neg         :in std_logic;

              idc_inc         :in std_logic;
              idc_dec         :in std_logic;

              idc_cp          :in std_logic;              
              idc_cpc         :in std_logic;
              idc_cpi         :in std_logic;
              idc_cpse        :in std_logic;                            

              idc_lsr         :in std_logic;
              idc_ror         :in std_logic;
              idc_asr         :in std_logic;
              idc_swap        :in std_logic;


-- DATA OUTPUT
              alu_data_out    : out std_logic_vector(7 downto 0);

-- FLAGS OUTPUT
              alu_c_flag_out  : out std_logic;
              alu_z_flag_out  : out std_logic;
              alu_n_flag_out  : out std_logic;
              alu_v_flag_out  : out std_logic;
              alu_s_flag_out  : out std_logic;
              alu_h_flag_out  : out std_logic
);

end component;


component reg_file is generic(ResetRegFile : boolean);
	                  port (
          reg_rd_in   : in std_logic_vector  (7 downto 0);
          reg_rd_out  : out std_logic_vector (7 downto 0);
          reg_rd_adr  : in std_logic_vector  (4 downto 0);
          reg_rr_out  : out std_logic_vector (7 downto 0);
          reg_rr_adr  : in std_logic_vector  (4 downto 0);
          reg_rd_wr   : in std_logic;

          post_inc    : in std_logic;                       -- POST INCREMENT FOR LD/ST INSTRUCTIONS
          pre_dec     : in std_logic;                        -- PRE DECREMENT FOR LD/ST INSTRUCTIONS
          reg_h_wr    : in std_logic;
          reg_h_out   : out std_logic_vector (15 downto 0);
          reg_h_adr   : in std_logic_vector (2 downto 0);    -- x,y,z
   		  reg_z_out   : out std_logic_vector (15 downto 0);  -- OUTPUT OF R31:R30 FOR LPM/ELPM/IJMP INSTRUCTIONS
		  

          clk         : in std_logic;
          nrst        : in std_logic

 );
end component;

component io_reg_file is port (
          clk         : in std_logic;
          nrst        : in std_logic;

          adr          : in std_logic_vector(5 downto 0);         
          iowe         : in std_logic;         
          dbusout      : in std_logic_vector(7 downto 0);         

          sreg_fl_in   : in std_logic_vector(7 downto 0);         
          sreg_out     : out std_logic_vector(7 downto 0);         

          sreg_fl_wr_en : in  std_logic_vector (7 downto 0);   --FLAGS WRITE ENABLE SIGNALS       

          spl_out      : out std_logic_vector(7 downto 0);         
          sph_out      : out std_logic_vector(7 downto 0);         
          sp_ndown_up  : in std_logic; -- DIRECTION OF CHANGING OF STACK POINTER SPH:SPL 0->UP(+) 1->DOWN(-)
          sp_en        : in std_logic; -- WRITE ENABLE(COUNT ENABLE) FOR SPH AND SPL REGISTERS
  
          rampz_out    : out std_logic_vector(7 downto 0)

 );
end component;


component bit_processor is port(
  
              clk             : in std_logic;            
              nrst            : in std_logic;            
              
              bit_num_r_io    : in std_logic_vector (2 downto 0);  -- BIT NUMBER FOR CBI/SBI/BLD/BST/SBRS/SBRC/SBIC/SBIS INSTRUCTIONS
              dbusin          : in  std_logic_vector(7 downto 0);  -- SBI/CBI/SBIS/SBIC  IN
              bitpr_io_out    : out std_logic_vector(7 downto 0);  -- SBI/CBI OUT        

              sreg_out        : in  std_logic_vector(7 downto 0);   -- BRBS/BRBC/BLD IN 
              branch     : in  std_logic_vector (2 downto 0);  -- NUMBER (0..7) OF BRANCH CONDITION FOR BRBS/BRBC INSTRUCTION


              bit_pr_sreg_out : out std_logic_vector(7 downto 0);   -- BCLR/BSET/BST(T-FLAG ONLY)             

              sreg_bit_num    : in std_logic_vector(2 downto 0);    -- BIT NUMBER FOR BCLR/BSET INSTRUCTIONS

              bld_op_out      : out std_logic_vector(7 downto 0);   -- BLD OUT (T FLAG)
              reg_rd_out      : in  std_logic_vector(7 downto 0);   -- BST/SBRS/SBRC IN    

              bit_test_op_out : out std_logic;                      -- OUTPUT OF SBIC/SBIS/SBRS/SBRC


-- OPERATION SIGNALS INPUTS

              -- INSTRUCTUIONS AND STATES

              idc_sbi         : in std_logic;
              sbi_st          : in std_logic;
              idc_cbi         : in std_logic;
              cbi_st          : in std_logic;

              idc_bld         : in std_logic;
              idc_bst         : in std_logic;
              idc_bset        : in std_logic;
              idc_bclr        : in std_logic;

              idc_sbic         : in std_logic;
              idc_sbis         : in std_logic;
              
              idc_sbrs         : in std_logic;
              idc_sbrc         : in std_logic;
              
              idc_brbs         : in std_logic;
              idc_brbc         : in std_logic;

              idc_reti         : in std_logic
                            
);

end component;

component io_adr_dec is port (
          adr          : in std_logic_vector(5 downto 0);         
          iore         : in std_logic;         
          dbusin_ext   : in std_logic_vector(7 downto 0);
          dbusin_int   : out std_logic_vector(7 downto 0);
                    
          spl_out      : in std_logic_vector(7 downto 0); 
          sph_out      : in std_logic_vector(7 downto 0);           
          sreg_out     : in std_logic_vector(7 downto 0);           
          rampz_out    : in std_logic_vector(7 downto 0));
end component;



-- *****************************************************************************************


signal sg_dbusin,sg_dbusout : std_logic_vector (7 downto 0) := (others =>'0');
signal sg_adr             : std_logic_vector (5 downto 0) := (others =>'0');      
signal sg_iowe,sg_iore    :std_logic  := '0';

-- SIGNALS FOR INSTRUCTION AND STATES

signal sg_idc_add,sg_idc_adc,sg_idc_adiw,sg_idc_sub,sg_idc_subi,sg_idc_sbc,sg_idc_sbci,sg_idc_sbiw,
       sg_adiw_st,sg_sbiw_st,sg_idc_and,sg_idc_andi,sg_idc_or,sg_idc_ori,sg_idc_eor,sg_idc_com,
       sg_idc_neg,sg_idc_inc,sg_idc_dec,sg_idc_cp,sg_idc_cpc,sg_idc_cpi,sg_idc_cpse,
       sg_idc_lsr,sg_idc_ror,sg_idc_asr,sg_idc_swap,sg_idc_sbi,sg_sbi_st,sg_idc_cbi,sg_cbi_st,
       sg_idc_bld,sg_idc_bst,sg_idc_bset,sg_idc_bclr,sg_idc_sbic,sg_idc_sbis,sg_idc_sbrs,sg_idc_sbrc,
       sg_idc_brbs,sg_idc_brbc,sg_idc_reti  : std_logic := '0';

signal sg_alu_data_r_in,sg_alu_data_d_in,sg_alu_data_out : std_logic_vector(7 downto 0) := (others =>'0');

signal sg_reg_rd_in,sg_reg_rd_out,sg_reg_rr_out : std_logic_vector  (7 downto 0) := (others =>'0');
signal sg_reg_rd_adr,sg_reg_rr_adr              : std_logic_vector  (4 downto 0) := (others =>'0');
signal sg_reg_h_out,sg_reg_z_out                : std_logic_vector (15 downto 0) := (others =>'0');
signal sg_reg_h_adr                             : std_logic_vector (2 downto 0) := (others =>'0');
signal sg_reg_rd_wr,sg_post_inc,
                       sg_pre_dec,sg_reg_h_wr   : std_logic  := '0';

signal sg_sreg_fl_in,sg_sreg_out,sg_sreg_fl_wr_en,
       sg_spl_out,sg_sph_out,sg_rampz_out                  : std_logic_vector(7 downto 0) := (others =>'0');

signal sg_sp_ndown_up,sg_sp_en : std_logic  := '0';


signal sg_bit_num_r_io,sg_branch,sg_sreg_bit_num  : std_logic_vector (2 downto 0) := (others =>'0');
signal sg_bitpr_io_out,sg_bit_pr_sreg_out,sg_sreg_flags,
       sg_bld_op_out,sg_reg_file_rd_in : std_logic_vector(7 downto 0) := (others =>'0');


signal sg_bit_test_op_out : std_logic  := '0';

signal sg_alu_c_flag_out,sg_alu_z_flag_out,sg_alu_n_flag_out,sg_alu_v_flag_out,
       sg_alu_s_flag_out,sg_alu_h_flag_out  : std_logic  := '0';


begin

main: component pm_fetch_dec port map
(

-- EXTERNAL INTERFACES OF THE CORE
clk      => cp2,
nrst     => ireset,
cpuwait  => cpuwait,

-- PROGRAM MEMORY PORTS
pc       => pc,    
inst     => inst,

-- I/O REGISTERS PORTS
adr      => sg_adr,
iore     => sg_iore,
iowe     => sg_iowe,

-- DATA MEMORY PORTS
ramadr   => ramadr,
ramre    => ramre,
ramwe    => ramwe,

dbusin   => sg_dbusin,
dbusout  => sg_dbusout,

-- INTERRUPTS PORT
irqlines => irqlines,
irqack   => irqack,
irqackad => irqackad,

-- END OF THE CORE INTERFACES


-- *********************************************************************************************
-- ******************** INTERFACES TO THE OTHER BLOCKS *****************************************
-- *********************************************************************************************


-- *********************************************************************************************
-- ******************** INTERFACES TO THE ALU  *************************************************
-- *********************************************************************************************
              alu_data_r_in   => sg_alu_data_r_in,
              alu_data_d_in   => sg_alu_data_d_in,

-- OPERATION SIGNALS INPUTS

              idc_add_out  => sg_idc_add,
              idc_adc_out  => sg_idc_adc,
              idc_adiw_out => sg_idc_adiw,
              idc_sub_out  => sg_idc_sub,
              idc_subi_out => sg_idc_subi,
              idc_sbc_out  => sg_idc_sbc,
              idc_sbci_out => sg_idc_sbci,
              idc_sbiw_out => sg_idc_sbiw,

              adiw_st_out  => sg_adiw_st,
              sbiw_st_out  => sg_sbiw_st,

              idc_and_out  => sg_idc_and,
              idc_andi_out => sg_idc_andi,
              idc_or_out   => sg_idc_or,
              idc_ori_out  => sg_idc_ori,
              idc_eor_out  => sg_idc_eor,
              idc_com_out  => sg_idc_com,
              idc_neg_out  => sg_idc_neg,

              idc_inc_out  => sg_idc_inc,
              idc_dec_out  => sg_idc_dec,

              idc_cp_out   => sg_idc_cp,
              idc_cpc_out  => sg_idc_cpc,
              idc_cpi_out  => sg_idc_cpi,
              idc_cpse_out => sg_idc_cpse,

              idc_lsr_out  => sg_idc_lsr,
              idc_ror_out  => sg_idc_ror,
              idc_asr_out  => sg_idc_asr,
              idc_swap_out => sg_idc_swap,


-- DATA OUTPUT
              alu_data_out => sg_alu_data_out,

-- FLAGS OUTPUT
              alu_c_flag_out => sg_alu_c_flag_out,
              alu_z_flag_out => sg_alu_z_flag_out,
              alu_n_flag_out => sg_alu_n_flag_out,
              alu_v_flag_out => sg_alu_v_flag_out,
              alu_s_flag_out => sg_alu_s_flag_out,
              alu_h_flag_out => sg_alu_h_flag_out,

-- *********************************************************************************************
-- ******************** INTERFACES TO THE GENERAL PURPOSE REGISTER FILE ************************
-- *********************************************************************************************
          reg_rd_in   => sg_reg_rd_in,
          reg_rd_out  => sg_reg_rd_out,
          reg_rd_adr  => sg_reg_rd_adr,
          reg_rr_out  => sg_reg_rr_out,
          reg_rr_adr  => sg_reg_rr_adr,
          reg_rd_wr   => sg_reg_rd_wr,

          post_inc    => sg_post_inc,
          pre_dec     => sg_pre_dec,
          reg_h_wr    => sg_reg_h_wr,
          reg_h_out   => sg_reg_h_out,
          reg_h_adr   => sg_reg_h_adr,
   		  reg_z_out   => sg_reg_z_out,

-- *********************************************************************************************
-- ******************** INTERFACES TO THE INPUT/OUTPUT REGISTER FILE ***************************
-- *********************************************************************************************

          sreg_fl_in    => sg_sreg_fl_in, --??   
          sreg_out      => sg_sreg_out,    

          sreg_fl_wr_en => sg_sreg_fl_wr_en,

          spl_out       => sg_spl_out,       
          sph_out       => sg_sph_out,       
          sp_ndown_up   => sg_sp_ndown_up,
          sp_en         => sg_sp_en,
  
          rampz_out     => sg_rampz_out,

-- *********************************************************************************************
-- ******************** INTERFACES TO THE INPUT/OUTPUT ADDRESS DECODER  ************************
-- *********************************************************************************************
          
--		  ram_data_in  : in std_logic_vector (7 downto 0);
--          adr          : in std_logic_vector(5 downto 0);         
--          iore         : in std_logic;       -- CORE SIGNAL         
--          ramre        : in std_logic;	   -- CORE SIGNAL         
--          dbusin       : out std_logic_vector(7 downto 0));	-- CORE SIGNAL         			 

-- *********************************************************************************************
-- ******************** INTERFACES TO THE BIT PROCESSOR   **************************************
-- *********************************************************************************************

              bit_num_r_io    => sg_bit_num_r_io,  
              bitpr_io_out    => sg_bitpr_io_out, 

              branch     => sg_branch, 

              bit_pr_sreg_out => sg_bit_pr_sreg_out,

              sreg_bit_num    => sg_sreg_bit_num, 

              bld_op_out      => sg_bld_op_out, 

              bit_test_op_out => sg_bit_test_op_out,


-- OPERATION SIGNALS INPUTS

              -- INSTRUCTUIONS AND STATES

              idc_sbi_out  => sg_idc_sbi,
              sbi_st_out   => sg_sbi_st,
              idc_cbi_out  => sg_idc_cbi,
              cbi_st_out   => sg_cbi_st,

              idc_bld_out  => sg_idc_bld,
              idc_bst_out  => sg_idc_bst,
              idc_bset_out => sg_idc_bset,
              idc_bclr_out => sg_idc_bclr,

              idc_sbic_out => sg_idc_sbic,
              idc_sbis_out => sg_idc_sbis,
              
              idc_sbrs_out => sg_idc_sbrs,
              idc_sbrc_out => sg_idc_sbrc,
              
              idc_brbs_out => sg_idc_brbs,
              idc_brbc_out => sg_idc_brbc,

              idc_reti_out => sg_idc_reti
				 
);




general_purpose_register_file: component reg_file 
		  generic map(ResetRegFile => TRUE)
	      port map (
          reg_rd_in   => sg_reg_rd_in,
          reg_rd_out  => sg_reg_rd_out,
          reg_rd_adr  => sg_reg_rd_adr,
          reg_rr_out  => sg_reg_rr_out,
          reg_rr_adr  => sg_reg_rr_adr,
          reg_rd_wr   => sg_reg_rd_wr,

          post_inc    => sg_post_inc,
          pre_dec     => sg_pre_dec,
          reg_h_wr    => sg_reg_h_wr,
          reg_h_out   => sg_reg_h_out,
          reg_h_adr   => sg_reg_h_adr,
   		  reg_z_out   => sg_reg_z_out,

          clk  => cp2,       
          nrst => ireset      

 );


bit_proc: component bit_processor port map(
  
              clk   => cp2,                   
              nrst => ireset,                  
              
              bit_num_r_io  => sg_bit_num_r_io,  
              dbusin        => sg_dbusin,   
              bitpr_io_out  => sg_bitpr_io_out,   

              sreg_out      => sg_sreg_out,   
              branch   => sg_branch,  

              bit_pr_sreg_out => sg_bit_pr_sreg_out,

              sreg_bit_num    => sg_sreg_bit_num,

              bld_op_out      => sg_bld_op_out,
              reg_rd_out      => sg_reg_rd_out,

              bit_test_op_out => sg_bit_test_op_out,


-- OPERATION SIGNALS INPUTS

              -- INSTRUCTUIONS AND STATES

              idc_sbi  => sg_idc_sbi,        
              sbi_st   => sg_sbi_st,       
              idc_cbi  => sg_idc_cbi,       
              cbi_st   => sg_cbi_st,       

              idc_bld  => sg_idc_bld,       
              idc_bst  => sg_idc_bst,       
              idc_bset => sg_idc_bset,       
              idc_bclr => sg_idc_bclr,       

              idc_sbic => sg_idc_sbic,       
              idc_sbis => sg_idc_sbis,       
              
              idc_sbrs => sg_idc_sbrs,        
              idc_sbrc => sg_idc_sbrc,        
              
              idc_brbs => sg_idc_brbs,        
              idc_brbc => sg_idc_brbc,        

              idc_reti => sg_idc_reti
);                      


io_dec:component io_adr_dec port map (
          adr          => sg_adr,
          iore         => sg_iore,
          dbusin_int   => sg_dbusin,			-- LOCAL DATA BUS OUTPUT
          dbusin_ext   => dbusin,               -- EXTERNAL DATA BUS INPUT
                   
          spl_out      => sg_spl_out,
          sph_out      => sg_sph_out,
          sreg_out     => sg_sreg_out,
          rampz_out    => sg_rampz_out
);

io_registers: component io_reg_file port map (
          clk        => cp2,
          nrst       => ireset,     

          adr        => sg_adr,       
          iowe       => sg_iowe,
          dbusout    => sg_dbusout,     

          sreg_fl_in => sg_sreg_fl_in,
          sreg_out   => sg_sreg_out,

          sreg_fl_wr_en => sg_sreg_fl_wr_en,

          spl_out  => sg_spl_out,    
          sph_out  => sg_sph_out,    
          sp_ndown_up => sg_sp_ndown_up, 
          sp_en     => sg_sp_en,   
  
          rampz_out => sg_rampz_out   
);



ALU:component alu_avr port map(

              alu_data_r_in => sg_alu_data_r_in,
              alu_data_d_in => sg_alu_data_d_in,
              
              alu_c_flag_in => sg_sreg_out(0),
              alu_z_flag_in => sg_sreg_out(1),


-- OPERATION SIGNALS INPUTS
              idc_add  => sg_idc_add,
              idc_adc  => sg_idc_adc,      
              idc_adiw => sg_idc_adiw,     
              idc_sub  => sg_idc_sub,     
              idc_subi => sg_idc_subi,     
              idc_sbc  => sg_idc_sbc,     
              idc_sbci => sg_idc_sbci,     
              idc_sbiw => sg_idc_sbiw,     

              adiw_st  => sg_adiw_st,     
              sbiw_st  => sg_sbiw_st,     

              idc_and  => sg_idc_and,     
              idc_andi => sg_idc_andi,     
              idc_or   => sg_idc_or,     
              idc_ori  => sg_idc_ori,     
              idc_eor  => sg_idc_eor,     
              idc_com  => sg_idc_com,     
              idc_neg  => sg_idc_neg,     

              idc_inc  => sg_idc_inc,     
              idc_dec  => sg_idc_dec,     

              idc_cp   => sg_idc_cp,     
              idc_cpc  => sg_idc_cpc,     
              idc_cpi  => sg_idc_cpi,    
              idc_cpse => sg_idc_cpse,     

              idc_lsr  => sg_idc_lsr,     
              idc_ror  => sg_idc_ror,      
              idc_asr  => sg_idc_asr,      
              idc_swap => sg_idc_swap,      


-- DATA OUTPUT
              alu_data_out => sg_alu_data_out,  

-- FLAGS OUTPUT
              alu_c_flag_out => sg_alu_c_flag_out,
              alu_z_flag_out => sg_alu_z_flag_out,
              alu_n_flag_out => sg_alu_n_flag_out,
              alu_v_flag_out => sg_alu_v_flag_out,
              alu_s_flag_out => sg_alu_s_flag_out,
              alu_h_flag_out => sg_alu_h_flag_out
);



adr      <= sg_adr;     
iowe     <= sg_iowe;
iore     <= sg_iore;
dbusout  <= sg_dbusout;

end struct;
