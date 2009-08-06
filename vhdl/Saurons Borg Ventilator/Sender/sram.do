onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /top_top_sch_tb/clk50
add wave -noupdate -format Logic /top_top_sch_tb/clk
add wave -noupdate -format Logic /top_top_sch_tb/clk100
add wave -noupdate -format Logic /top_top_sch_tb/sram_oe
add wave -noupdate -format Logic /top_top_sch_tb/sram_we
add wave -noupdate -format Literal /top_top_sch_tb/sram_adr
add wave -noupdate -format Literal /top_top_sch_tb/sram_1_io
add wave -noupdate -format Logic /top_top_sch_tb/sram_1_ce
add wave -noupdate -format Logic /top_top_sch_tb/sram_1_ub
add wave -noupdate -format Logic /top_top_sch_tb/sram_1_lb
add wave -noupdate -format Logic /top_top_sch_tb/sram_2_ce
add wave -noupdate -format Literal /top_top_sch_tb/sram_2_io
add wave -noupdate -format Logic /top_top_sch_tb/sram_2_ub
add wave -noupdate -format Logic /top_top_sch_tb/sram_2_lb
add wave -noupdate -format Literal /top_top_sch_tb/winkel
add wave -noupdate -format Literal /top_top_sch_tb/winkel_ram
add wave -noupdate -format Logic /top_top_sch_tb/b10code_k1
add wave -noupdate -format Logic /top_top_sch_tb/b10code_k2
add wave -noupdate -format Logic /top_top_sch_tb/b10code_k3
add wave -noupdate -format Logic /top_top_sch_tb/b10code_k4
add wave -noupdate -format Literal /top_top_sch_tb/sram_read
add wave -noupdate -format Literal /top_top_sch_tb/sram_pos
add wave -noupdate -format Literal /top_top_sch_tb/decode_data_k1
add wave -noupdate -format Literal /top_top_sch_tb/decode_data_k2
add wave -noupdate -format Literal /top_top_sch_tb/decode_data_k3
add wave -noupdate -format Literal /top_top_sch_tb/decode_data_k4
add wave -noupdate -format Literal /top_top_sch_tb/decode_data_k1_last
add wave -noupdate -format Literal /top_top_sch_tb/decode_data_k2_last
add wave -noupdate -format Literal /top_top_sch_tb/decode_data_k3_last
add wave -noupdate -format Literal /top_top_sch_tb/decode_data_k4_last
add wave -noupdate -format Literal /top_top_sch_tb/decode_counter_k1
add wave -noupdate -format Literal /top_top_sch_tb/decode_counter_k2
add wave -noupdate -format Literal /top_top_sch_tb/decode_counter_k3
add wave -noupdate -format Literal /top_top_sch_tb/decode_counter_k4
add wave -noupdate -format Logic /top_top_sch_tb/decode_rdy_k1
add wave -noupdate -format Logic /top_top_sch_tb/decode_rdy_k2
add wave -noupdate -format Logic /top_top_sch_tb/decode_rdy_k3
add wave -noupdate -format Logic /top_top_sch_tb/decode_rdy_k4
add wave -noupdate -format Logic /top_top_sch_tb/decode_rdy_a_k1
add wave -noupdate -format Logic /top_top_sch_tb/decode_rdy_a_k2
add wave -noupdate -format Logic /top_top_sch_tb/decode_rdy_a_k3
add wave -noupdate -format Logic /top_top_sch_tb/decode_rdy_a_k4
add wave -noupdate -format Logic /top_top_sch_tb/decode_synchron_k1
add wave -noupdate -format Logic /top_top_sch_tb/decode_synchron_k2
add wave -noupdate -format Logic /top_top_sch_tb/decode_synchron_k3
add wave -noupdate -format Logic /top_top_sch_tb/decode_synchron_k4
add wave -noupdate -format Literal /top_top_sch_tb/visual_abstand
add wave -noupdate -format Literal /top_top_sch_tb/visual_abstand_k1
add wave -noupdate -format Literal /top_top_sch_tb/visual_abstand_k2
add wave -noupdate -format Literal /top_top_sch_tb/visual_abstand_k3
add wave -noupdate -format Literal /top_top_sch_tb/visual_abstand_k4
add wave -noupdate -format Literal /top_top_sch_tb/visual_abstand_a_k1
add wave -noupdate -format Literal /top_top_sch_tb/visual_abstand_a_k2
add wave -noupdate -format Literal /top_top_sch_tb/visual_abstand_a_k3
add wave -noupdate -format Literal /top_top_sch_tb/visual_abstand_a_k4
add wave -noupdate -format Literal /top_top_sch_tb/visual_winkel
add wave -noupdate -format Literal /top_top_sch_tb/visual_winkel_k1
add wave -noupdate -format Literal /top_top_sch_tb/visual_winkel_k2
add wave -noupdate -format Literal /top_top_sch_tb/visual_winkel_k3
add wave -noupdate -format Literal /top_top_sch_tb/visual_winkel_k4
add wave -noupdate -format Literal /top_top_sch_tb/visual_color
add wave -noupdate -format Literal /top_top_sch_tb/visual_color_k1
add wave -noupdate -format Literal /top_top_sch_tb/visual_color_k2
add wave -noupdate -format Literal /top_top_sch_tb/visual_color_k3
add wave -noupdate -format Literal /top_top_sch_tb/visual_color_k4
add wave -noupdate -format Literal /top_top_sch_tb/colordatae
add wave -noupdate -format Literal /top_top_sch_tb/xe
add wave -noupdate -format Literal /top_top_sch_tb/ye
add wave -noupdate -format Literal /top_top_sch_tb/colordatai
add wave -noupdate -format Literal /top_top_sch_tb/xi
add wave -noupdate -format Literal /top_top_sch_tb/yi
add wave -noupdate -format Literal /top_top_sch_tb/colordatal
add wave -noupdate -format Literal /top_top_sch_tb/xl
add wave -noupdate -format Literal /top_top_sch_tb/yl
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {318534296 ps} 0}
configure wave -namecolwidth 254
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
configure wave -timelineunits ns
update
WaveRestoreZoom {348821173 ps} {348877961 ps}
