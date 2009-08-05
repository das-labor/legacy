onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /top_top_sch_tb/clk20
add wave -noupdate -format Literal -radix unsigned /top_top_sch_tb/visual_winkel
add wave -noupdate -format Literal -radix unsigned /top_top_sch_tb/visual_abstand
add wave -noupdate -format Literal -radix hexadecimal /top_top_sch_tb/visual_color
add wave -noupdate -format Literal -radix unsigned /top_top_sch_tb/visual_winkel_k1
add wave -noupdate -format Literal -radix unsigned /top_top_sch_tb/visual_abstand_k1
add wave -noupdate -format Literal -radix hexadecimal /top_top_sch_tb/visual_color_k1
add wave -noupdate -format Literal -radix unsigned /top_top_sch_tb/visual_winkel_k3
add wave -noupdate -format Literal -radix unsigned /top_top_sch_tb/visual_abstand_k3
add wave -noupdate -format Logic /top_top_sch_tb/b10code_k3
add wave -noupdate -format Logic /top_top_sch_tb/b10code_k1
add wave -noupdate -format Literal -radix hexadecimal /top_top_sch_tb/visual_color_k3
add wave -noupdate -format Literal -radix hexadecimal /top_top_sch_tb/decode_data_k1
add wave -noupdate -format Literal -radix unsigned /top_top_sch_tb/decode_counter_k1
add wave -noupdate -format Literal -radix hexadecimal /top_top_sch_tb/decode_data_k3
add wave -noupdate -format Literal -radix unsigned /top_top_sch_tb/decode_counter_k3
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {912730000 ps} 0}
configure wave -namecolwidth 289
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {187474728 ps} {189002848 ps}
