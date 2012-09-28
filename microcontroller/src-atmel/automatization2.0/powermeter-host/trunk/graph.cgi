#!/usr/bin/env python

"""
 Return a PNG graph from rrd files...
"""

import cgitb
#cgitb.enable()
cgitb.enable(display=0, logdir="/tmp")

import re
import os
import cgi
import sys
from string import Template

graph_header = """
rrdtool graph -                   
    -a PNG -w $WIDTH -h $HEIGHT  
    --start='$START' --end='$END'
"""

THEME_TAGS = ['BACK', 'SHADEA', 'SHADEB', 'FONT',
    'CANVAS', 'GRID', 'MGRID', 'FRAME', 'ARROW']

graph_templates = {
    "power-I": """
        --title="Strom [A]"
        -l 0
        'DEF:I1=$POWERRRD/powermeter.rrd:I1:AVERAGE'
        'DEF:I1_=$POWERRRD/powermeter.rrd:I1:MAX'        
        'DEF:I2=$POWERRRD/powermeter.rrd:I2:AVERAGE'    
        'DEF:I2_=$POWERRRD/powermeter.rrd:I2:MAX'      
        'DEF:I3=$POWERRRD/powermeter.rrd:I3:AVERAGE'  
        'DEF:I3_=$POWERRRD/powermeter.rrd:I3:MAX'
        'AREA:I1#$ACOLOR1:'
        'AREA:I2#$ACOLOR2::STACK'
        'AREA:I3#$ACOLOR3::STACK'
        'LINE1:I1#$LCOLOR1:I1'
        'LINE1:I2#$LCOLOR2:I2:STACK'
        'LINE1:I3#$LCOLOR3:I3:STACK'
    """,
    "power-P": """
        --title="Wirkleistung [W]"
        -E
        -l 0
        'DEF:P1=$POWERRRD/powermeter.rrd:P1:AVERAGE'
        'DEF:P1_=$POWERRRD/powermeter.rrd:P1:MAX'
        'DEF:P2=$POWERRRD/powermeter.rrd:P2:AVERAGE'
        'DEF:P2_=$POWERRRD/powermeter.rrd:P2:MAX'
        'DEF:P3=$POWERRRD/powermeter.rrd:P3:AVERAGE'
        'DEF:P3_=$POWERRRD/powermeter.rrd:P3:MAX'
        'AREA:P1#$ACOLOR1:'
        'AREA:P2#$ACOLOR2::STACK'
        'AREA:P3#$ACOLOR3::STACK'
        'LINE1:P1#$LCOLOR1:P1'
        'LINE1:P2#$LCOLOR2:P2:STACK'
        'LINE1:P3#$LCOLOR3:P3:STACK'
        'CDEF:P=P1,P2,P3,+,+'   
        'CDEF:P_=P1_,P2_,P3_,+,+'
        'GPRINT:P:LAST:Aktuell %6.2lf %sW, ' 
        'GPRINT:P:AVERAGE:Durchschnitt %6.2lf %sW'
    """,
    "power-Q": """
        --title="Blindleistung [W]"
        -l 0
        'DEF:P1=$POWERRRD/powermeter.rrd:P1:AVERAGE'       
        'DEF:P1_=$POWERRRD/powermeter.rrd:P1:MAX'          
        'DEF:P2=$POWERRRD/powermeter.rrd:P2:AVERAGE'       
        'DEF:P2_=$POWERRRD/powermeter.rrd:P2:MAX'          
        'DEF:P3=$POWERRRD/powermeter.rrd:P3:AVERAGE'       
        'DEF:P3_=$POWERRRD/powermeter.rrd:P3:MAX'          
        'DEF:S1=$POWERRRD/powermeter.rrd:S1:AVERAGE'       
        'DEF:S1_=$POWERRRD/powermeter.rrd:S1:MAX'          
        'DEF:S2=$POWERRRD/powermeter.rrd:S2:AVERAGE'       
        'DEF:S2_=$POWERRRD/powermeter.rrd:S2:MAX'          
        'DEF:S3=$POWERRRD/powermeter.rrd:S3:AVERAGE'       
        'DEF:S3_=$POWERRRD/powermeter.rrd:S3:MAX'          
        'CDEF:Q1=S1,P1,-'                        
        'CDEF:Q2=S2,P2,-'                        
        'CDEF:Q3=S3,P3,-'                        
        'CDEF:Q1_=S1_,P1_,-'                     
        'CDEF:Q2_=S2_,P2_,-'                     
        'CDEF:Q3_=S3_,P3_,-'                     
        'AREA:Q1#$ACOLOR1:'                     
        'AREA:Q2#$ACOLOR2::STACK'               
        'AREA:Q3#$ACOLOR3::STACK'               
        'LINE1:Q1#$LCOLOR1:Q1'                   
        'LINE1:Q2#$LCOLOR2:Q2:STACK'             
        'LINE1:Q3#$LCOLOR3:Q3:STACK'             
        'CDEF:Q=Q1,Q2,Q3,+,+'                    
        'GPRINT:Q:LAST:Aktuell %6.2lf %sW, '        
        'GPRINT:Q:AVERAGE:Durchschnitt %6.2lf %sW'
    """,
    "power-U": """
        --title="Spannung [V]" 
        -l 220 -u 250 -r
        'DEF:U1=$POWERRRD/powermeter.rrd:U1:AVERAGE'       
        'DEF:U1_=$POWERRRD/powermeter.rrd:U1:MAX'          
        'DEF:U2=$POWERRRD/powermeter.rrd:U2:AVERAGE'       
        'DEF:U2_=$POWERRRD/powermeter.rrd:U2:MAX'          
        'DEF:U3=$POWERRRD/powermeter.rrd:U3:AVERAGE'       
        'DEF:U3_=$POWERRRD/powermeter.rrd:U3:MAX'          
        'LINE1:U1#$LCOLOR1:U1'                   
        'LINE1:U2#$LCOLOR2:U2'                
        'LINE1:U3#$LCOLOR3:U3'                
        'GPRINT:U1:LAST:U1=%5.2lf V, '        
        'GPRINT:U2:LAST:U2=%5.2lf V, '        
        'GPRINT:U3:LAST:U3=%5.2lf V'        
    """,
    "memory": """
        --title="Speicherauslastung"
        -l 0
        "DEF:buffered=$COLLECTDRRD/$HOST/memory/memory-buffered.rrd:value:AVERAGE"
        "DEF:cached=$COLLECTDRRD/$HOST/memory/memory-cached.rrd:value:AVERAGE"
        "DEF:free=$COLLECTDRRD/$HOST/memory/memory-free.rrd:value:AVERAGE"
        "DEF:used=$COLLECTDRRD/$HOST/memory/memory-used.rrd:value:AVERAGE"
        "AREA:used#$ACOLOR1::"
        "AREA:buffered#$ACOLOR2::STACK"
        "AREA:cached#$ACOLOR4::STACK"
        "AREA:free#$ACOLOR3::STACK"
        "LINE1:used#$LCOLOR1:used:"
        "LINE1:buffered#$LCOLOR2:buffered:STACK"
        "LINE1:cached#$LCOLOR4:cached:STACK"
        "LINE1:free#$LCOLOR3:free:STACK"
    """,
    "cpu1": """
        --title="CPU Nutzung"
        DEF:cpu-idle=$COLLECTDRRD/$HOST/cpu-0/cpu-idle.rrd:value:AVERAGE
        DEF:cpu-interrupt=$COLLECTDRRD/$HOST/cpu-0/cpu-interrupt.rrd:value:AVERAGE
        DEF:cpu-nice=$COLLECTDRRD/$HOST/cpu-0/cpu-nice.rrd:value:AVERAGE           
        DEF:cpu-softirq=$COLLECTDRRD/$HOST/cpu-0/cpu-softirq.rrd:value:AVERAGE     
        DEF:cpu-steal=$COLLECTDRRD/$HOST/cpu-0/cpu-steal.rrd:value:AVERAGE         
        DEF:cpu-system=$COLLECTDRRD/$HOST/cpu-0/cpu-system.rrd:value:AVERAGE       
        DEF:cpu-user=$COLLECTDRRD/$HOST/cpu-0/cpu-user.rrd:value:AVERAGE           
        AREA:cpu-steal#$ACOLOR1::                     
        AREA:cpu-system#$ACOLOR1::STACK               
        AREA:cpu-interrupt#$ACOLOR2::STACK            
        AREA:cpu-softirq#$ACOLOR2::STACK              
        AREA:cpu-user#$ACOLOR4::STACK                 
        AREA:cpu-nice#$ACOLOR3::STACK                 
        LINE1:cpu-steal#$LCOLOR1::               
        LINE1:cpu-system#$LCOLOR1:system:STACK        
        LINE1:cpu-interrupt#$LCOLOR2::STACK  
        LINE1:cpu-softirq#$LCOLOR2:irq:STACK     
        LINE1:cpu-user#$LCOLOR4:user:STACK            
        LINE1:cpu-nice#$LCOLOR3:nice:STACK
    """, 
 
    "cpu4": """
        --title="CPU Nutzung"
        DEF:cpu-0-idle=$COLLECTDRRD/$HOST/cpu-0/cpu-idle.rrd:value:AVERAGE
        DEF:cpu-0-interrupt=$COLLECTDRRD/$HOST/cpu-0/cpu-interrupt.rrd:value:AVERAGE
        DEF:cpu-0-nice=$COLLECTDRRD/$HOST/cpu-0/cpu-nice.rrd:value:AVERAGE           
        DEF:cpu-0-softirq=$COLLECTDRRD/$HOST/cpu-0/cpu-softirq.rrd:value:AVERAGE     
        DEF:cpu-0-steal=$COLLECTDRRD/$HOST/cpu-0/cpu-steal.rrd:value:AVERAGE         
        DEF:cpu-0-system=$COLLECTDRRD/$HOST/cpu-0/cpu-system.rrd:value:AVERAGE       
        DEF:cpu-0-user=$COLLECTDRRD/$HOST/cpu-0/cpu-user.rrd:value:AVERAGE           
        DEF:cpu-1-idle=$COLLECTDRRD/$HOST/cpu-1/cpu-idle.rrd:value:AVERAGE           
        DEF:cpu-1-interrupt=$COLLECTDRRD/$HOST/cpu-1/cpu-interrupt.rrd:value:AVERAGE 
        DEF:cpu-1-nice=$COLLECTDRRD/$HOST/cpu-1/cpu-nice.rrd:value:AVERAGE           
        DEF:cpu-1-softirq=$COLLECTDRRD/$HOST/cpu-1/cpu-softirq.rrd:value:AVERAGE     
        DEF:cpu-1-steal=$COLLECTDRRD/$HOST/cpu-1/cpu-steal.rrd:value:AVERAGE         
        DEF:cpu-1-system=$COLLECTDRRD/$HOST/cpu-1/cpu-system.rrd:value:AVERAGE       
        DEF:cpu-1-user=$COLLECTDRRD/$HOST/cpu-1/cpu-user.rrd:value:AVERAGE           
        DEF:cpu-2-idle=$COLLECTDRRD/$HOST/cpu-2/cpu-idle.rrd:value:AVERAGE           
        DEF:cpu-2-interrupt=$COLLECTDRRD/$HOST/cpu-2/cpu-interrupt.rrd:value:AVERAGE 
        DEF:cpu-2-nice=$COLLECTDRRD/$HOST/cpu-2/cpu-nice.rrd:value:AVERAGE           
        DEF:cpu-2-softirq=$COLLECTDRRD/$HOST/cpu-2/cpu-softirq.rrd:value:AVERAGE     
        DEF:cpu-2-steal=$COLLECTDRRD/$HOST/cpu-2/cpu-steal.rrd:value:AVERAGE         
        DEF:cpu-2-system=$COLLECTDRRD/$HOST/cpu-2/cpu-system.rrd:value:AVERAGE       
        DEF:cpu-2-user=$COLLECTDRRD/$HOST/cpu-2/cpu-user.rrd:value:AVERAGE           
        DEF:cpu-3-idle=$COLLECTDRRD/$HOST/cpu-2/cpu-idle.rrd:value:AVERAGE           
        DEF:cpu-3-interrupt=$COLLECTDRRD/$HOST/cpu-3/cpu-interrupt.rrd:value:AVERAGE 
        DEF:cpu-3-nice=$COLLECTDRRD/$HOST/cpu-3/cpu-nice.rrd:value:AVERAGE           
        DEF:cpu-3-softirq=$COLLECTDRRD/$HOST/cpu-3/cpu-softirq.rrd:value:AVERAGE     
        DEF:cpu-3-steal=$COLLECTDRRD/$HOST/cpu-3/cpu-steal.rrd:value:AVERAGE         
        DEF:cpu-3-system=$COLLECTDRRD/$HOST/cpu-3/cpu-system.rrd:value:AVERAGE       
        DEF:cpu-3-user=$COLLECTDRRD/$HOST/cpu-3/cpu-user.rrd:value:AVERAGE           
        CDEF:cpu-idle=cpu-0-idle,cpu-1-idle,cpu-2-idle,cpu-3-idle,+,+,+
        CDEF:cpu-interrupt=cpu-0-interrupt,cpu-1-interrupt,cpu-2-interrupt,cpu-3-interrupt,+,+,+
        CDEF:cpu-nice=cpu-0-nice,cpu-1-nice,cpu-2-nice,cpu-3-nice,+,+,+
        CDEF:cpu-softirq=cpu-0-softirq,cpu-1-softirq,cpu-2-softirq,cpu-3-softirq,+,+,+
        CDEF:cpu-steal=cpu-0-steal,cpu-1-steal,cpu-2-steal,cpu-3-steal,+,+,+
        CDEF:cpu-system=cpu-0-system,cpu-1-system,cpu-2-system,cpu-3-system,+,+,+
        CDEF:cpu-user=cpu-0-user,cpu-1-user,cpu-2-user,cpu-3-user,+,+,+
        AREA:cpu-steal#$ACOLOR1::                     
        AREA:cpu-system#$ACOLOR1::STACK               
        AREA:cpu-interrupt#$ACOLOR2::STACK            
        AREA:cpu-softirq#$ACOLOR2::STACK              
        AREA:cpu-user#$ACOLOR4::STACK                 
        AREA:cpu-nice#$ACOLOR3::STACK                 
        LINE1:cpu-steal#94541FC0:steal:               
        LINE1:cpu-system#94541FC0:system:STACK        
        LINE1:cpu-interrupt#94541FC0:interrupt:STACK  
        LINE1:cpu-softirq#94541FC0:soft-irq:STACK     
        LINE1:cpu-user#948E1FC0:user:STACK            
        LINE1:cpu-nice#1F9454C0:nice:STACK
    """, 
    "can-U": """
        --title="CAN Spannung [V]"
        -l 0
        'DEF:U=$POWERRRD/gateway-power.rrd:U:AVERAGE'
        'DEF:U_=$POWERRRD/gateway-power.rrd:U:MAX'        
        'AREA:U_#$ACOLOR3:'
        'LINE1:U#$LCOLOR3:'
    """,
    "can-I": """
        --title="CAN Strom [A]"
        -l 0
        'DEF:I=$POWERRRD/gateway-power.rrd:I:AVERAGE'
        'DEF:I_=$POWERRRD/gateway-power.rrd:I:MAX'        
        'AREA:I_#$ACOLOR2:'
        'LINE1:I#$LCOLOR2:'
    """,
    "can-P": """
        --title="CAN Leistung [W]"
        -l 0
        'DEF:U=$POWERRRD/gateway-power.rrd:U:AVERAGE'
        'DEF:I=$POWERRRD/gateway-power.rrd:I:AVERAGE'
        'CDEF:P=U,I,*'
        'AREA:P#$ACOLOR1:'
        'LINE1:P#$LCOLOR1:'
    """,
    "can-pkts": """
        --title="CAN Durchsatz [1/s]"
        -l 0
        'DEF:RXpkts=$POWERRRD/gateway-pstats.rrd:RXpkts:AVERAGE'
        'DEF:RXpkts_=$POWERRRD/gateway-pstats.rrd:RXpkts:MAX'
        'DEF:TXpkts=$POWERRRD/gateway-pstats.rrd:TXpkts:AVERAGE'
        'DEF:TXpkts_=$POWERRRD/gateway-pstats.rrd:TXpkts:MAX'
        'AREA:RXpkts_#$ACOLOR1::'
        'AREA:TXpkts_#$ACOLOR2::STACK'
        'LINE1:RXpkts#$LCOLOR1::'
        'LINE1:TXpkts#$LCOLOR2::STACK'
    """,
    "can-bytes": """
        --title="CAN Durchsatz [Bytes/s]"
        -l 0
        'DEF:RX=$POWERRRD/gateway-pstats.rrd:RXbytes:AVERAGE'
        'DEF:RX_=$POWERRRD/gateway-pstats.rrd:RXbytes:MAX'
        'DEF:TX=$POWERRRD/gateway-pstats.rrd:TXbytes:AVERAGE'
        'DEF:TX_=$POWERRRD/gateway-pstats.rrd:TXbytes:MAX'
        'AREA:RX_#$ACOLOR1::'
        'AREA:TX_#$ACOLOR2::STACK'
        'LINE1:RX#$LCOLOR1::'
        'LINE1:TX#$LCOLOR2::STACK'
    """,
#        -l -2000000
#        -u 500000
    "uplink": """
        --title="DSL Uplink [Bytes/s]"
        -E
        -u 128000
        -l -1600000
        'DEF:rx=$COLLECTDRRD/casino/interface/if_octets-ppp0.rrd:rx:AVERAGE'
        'DEF:rx_=$COLLECTDRRD/casino/interface/if_octets-ppp0.rrd:rx:MAX'
        'DEF:tx=$COLLECTDRRD/casino/interface/if_octets-ppp0.rrd:tx:AVERAGE'
        'DEF:tx_=$COLLECTDRRD/casino/interface/if_octets-ppp0.rrd:tx:MAX'
        'DEF:rx6=$COLLECTDRRD/casino/interface/if_octets-sixxs.rrd:rx:AVERAGE'
        'DEF:rx6_=$COLLECTDRRD/casino/interface/if_octets-sixxs.rrd:rx:MAX'
        'DEF:tx6=$COLLECTDRRD/casino/interface/if_octets-sixxs.rrd:tx:AVERAGE'
        'DEF:tx6_=$COLLECTDRRD/casino/interface/if_octets-sixxs.rrd:tx:MAX'
        'CDEF:mrx4=rx,rx6,-,-1,*'
        'CDEF:mrx4_=rx_,rx6_,-,-1,*'
        'CDEF:tx4=tx,tx6,-'
        'CDEF:tx4_=tx_,tx6_,-'
        'CDEF:mrx6=rx6,-1,*'
        'CDEF:mrx6_=rx6_,-1,*'
        'AREA:tx4#$ACOLOR4:'
        'AREA:tx6#$BCOLOR4::STACK'
        'AREA:mrx4#$ACOLOR2:'
        'AREA:mrx6#$BCOLOR2::STACK'
        'LINE1:tx4#$LCOLOR4:TX IPv4'
        'LINE1:tx6#$LCOLOR4:TX IPv6:STACK'
        'LINE1:mrx4#$LCOLOR2:RX IPv4'
        'LINE1:mrx6#$LCOLOR2:RX IPv6:STACK'
        'GPRINT:tx:LAST:TX=%5.2lf %sB/s, '        
        'GPRINT:rx:LAST:RX=%5.2lf %sB/s, '        
    """,
}

themes = {
    "white": {
        "LCOLOR1": "94541FC0",
        "ACOLOR1": "94541F40",
        "LCOLOR2": "948E1FC0",
        "ACOLOR2": "948E1F40",
        "LCOLOR3": "1F9454C0",
        "ACOLOR3": "1F945440",
        "LCOLOR4": "1F948EC0",
        "ACOLOR4": "1F948E40",
        "LCOLOR5": "541F94C0",
        "ACOLOR5": "541F9440",
        "LCOLOR6": "8E1F94C0",
        "ACOLOR6": "8E1F9440"
    },
    "black": {
        "BACK":    "000000",
        "SHADEA":  "000000",
        "SHADEB":  "000000",
        "FONT":    "dddddd",
        "CANVAS":  "202020",
        "GRID":    "666666",
        "MGRID":   "aaaaaa",
        "FRAME":   "202020",
        "ARROW":   "FFFFFF",
        "LCOLOR1": "95EC00",
        "BCOLOR1": "80B12C",
        "ACOLOR1": "619A00",
        "LCOLOR2": "028E9B",
        "BCOLOR2": "1E6D74",
        "ACOLOR2": "015C65",
        "LCOLOR3": "FF7800",
        "BCOLOR3": "BFAA30",
        "ACOLOR3": "A68D00",
        "LCOLOR4": "D2006B",
        "BCOLOR4": "9D2763",
        "ACOLOR4": "880045"
    },
    "black2": {
        "BACK":    "000000",
        "SHADEA":  "000000",
        "SHADEB":  "000000",
        "FONT":    "dddddd",
        "CANVAS":  "202020",
        "GRID":    "666666",
        "MGRID":   "aaaaaa",
        "FRAME":   "202020",
        "ARROW":   "FFFFFF",
        "LCOLOR1": "00FF00",
        "ACOLOR1": "408040",
        "LCOLOR2": "0000FF",
        "ACOLOR2": "404080",
        "LCOLOR3": "FFFF00",
        "ACOLOR3": "808040",
        "LCOLOR4": "FF0000",
        "ACOLOR4": "804040"
    }
}



if __name__ == "__main__":

    subst = {
        "WIDTH":       "600",
        "HEIGHT":      "150",
        "HOST":        "kvm",
        "START":       "-1h",
        "END":         "-0s",
        "POWERRRD":    "/home/joerg/automatization2.0/powermeter-host/trunk/",
        "COLLECTDRRD": "/var/lib/collectd/rrd/",
    }

    form = cgi.FieldStorage()
    
    def get_arg(name, pattern, default=None):
        if name not in form:
            return default
        value = form.getfirst(name)
        if re.match("^"+pattern+"$", value):
            return value
        else: 
            raise ValueError("argument %s contains illegal characters" % name)

    # Graph type
    graph = get_arg("graph", "[\w-]{1,20}", "power-P")
    if graph not in graph_templates:
        raise ValueError("Unknown graph type")

    # Host?
    subst["HOST"] = get_arg("host", "[\w-]{1,20}", "kvm")


    # Select theme
    theme = get_arg("theme", "[\w-]{1,20}", "white")
    theme_subs = themes[theme]

    # Start time
    start = get_arg("start", "-\d{1,5}[\w]{1,3}", "-30min")
    subst["START"] = start

    verbose = False
    if "v" in form:
        verbose = True

    # Construct rrd command
    cmd = graph_header
    cmd += graph_templates[graph]
        
    # Theme tags
    for tag in THEME_TAGS:
        if tag in theme_subs:
            cmd += " -c %s#%s\n" % (tag, theme_subs[tag])

    cmd = cmd.replace("\n", "")
    subst.update(theme_subs)

    # Substitute
    cmd_tmpl = Template(cmd)
    cmd = cmd_tmpl.substitute(subst)

    # Generate out
    if verbose:
        print "Content-Type: text/html"     # HTML is following
        print                               # blank line, end of headers
        print cmd
    else:
        print "Content-Type: image/png"     # HTML is following
        print                               # blank line, end of headers
        sys.stdout.flush()
        os.system(cmd)

