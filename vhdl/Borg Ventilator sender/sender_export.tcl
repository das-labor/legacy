# 
# ProjectNavigator SourceControl export script
#
# This script is generated and executed by ProjectNavigator
# to copy files related to the current project to a
# destination staging area.
#
# The list of files being exported, as controled by
# GUI settings, is placed in the variable export_files.
# Each file will be passed to the procedure CopyOut.  This
# procedure will determine the exact destination, and will
# attempt the file copy if the source and destination are different.
#
# This script is not indended for direct customer editing.
#
# Copyright 2006, Xilinx, Inc.
#
#
# CopyOut -- copy a source file to the staging area, with
#            extra options for files remote (outside of) the
#            project directory.
proc CopyOut { srcfile staging_area copy_option } {
   set report_errors true
   if { $copy_option == "flatten" } {
       set dest [ file join $staging_area [ file tail $srcfile ] ]
   } elseif { $copy_option == "nop" } {
       set dest  $staging_area
       set report_errors false
   } elseif { [ file pathtype $srcfile ] != "relative" } {
       set srcfile2 [ string map {: _} $srcfile ]
       set dest [ file join $staging_area absolute $srcfile2 ]
   } elseif { [ expr { $copy_option == "absremote" } && { [string equal -length 2 $srcfile ".." ] } ] } {
       set dest [ file join $staging_area outside_relative [ string map {.. up} $srcfile ] ]
   } else {
       set srcfile2 [ string map {: _} $srcfile ]
       set dest [ file join $staging_area $srcfile2 ]
   }

   set dest [ file normalize $dest ]
   set src [ file normalize $srcfile ]

   if { [ expr { [ string equal $dest $src ] == 0 } && { [ file exists $src ] } ] } {
      file mkdir [ file dirname $dest ]
      if { [catch { file copy -force $src $dest } ] } {
         if { $report_errors } { puts "Could not copy $src to $dest." }
      }
   }
}

# change to the working directory
set old_working_dir [pwd]
cd [file normalize {D:/borg_sender/sender} ]
set copy_option relative
set staging_area "D:/borg_sender/sender"
set export_files { 
          "Daten_schieber.sym" 
          "Daten_schieber.vhd" 
          "Picoblaze.sym" 
          "Picoblaze.vhd" 
          "b83.sym" 
          "b84.sym" 
          "b84.vhd" 
          "clk20.sym" 
          "clk20.xaw" 
          "clk20_arwz.ucf" 
          "clk_arwz.ucf" 
          "decoder_b8b10b.asy" 
          "decoder_b8b10b.ngc" 
          "decoder_b8b10b.sym" 
          "decoder_b8b10b.v" 
          "decoder_b8b10b.veo" 
          "decoder_b8b10b.vhd" 
          "decoder_b8b10b.vho" 
          "decoder_b8b10b.xco" 
          "display2.sym" 
          "display2.vhd" 
          "encoder_b8b10b.asy" 
          "encoder_b8b10b.ngc" 
          "encoder_b8b10b.sym" 
          "encoder_b8b10b.v" 
          "encoder_b8b10b.veo" 
          "encoder_b8b10b.vhd" 
          "encoder_b8b10b.vho" 
          "encoder_b8b10b.xco" 
          "f_eins.vhd" 
          "f_null.sym" 
          "f_null.vhd" 
          "ioports.ucf" 
          "jitter_messen.vhd" 
          "kcpsm3.vhd" 
          "korrekturdaten.asy" 
          "korrekturdaten.ngc" 
          "korrekturdaten.sym" 
          "korrekturdaten.v" 
          "korrekturdaten.veo" 
          "korrekturdaten.vhd" 
          "korrekturdaten.vho" 
          "korrekturdaten.xco" 
          "packed_write.vhd" 
          "packet_read.sym" 
          "packet_read.vhd" 
          "packet_read_guide.ncd" 
          "packet_write.sym" 
          "packet_write_guide.ncd" 
          "pb_rom.sym" 
          "picoblaze_rom.vhd" 
          "takt.sym" 
          "takt.xaw" 
          "takt_arwz.ucf" 
          "tb_datenschieber.tbw" 
          "tb_datenschieber.xwv" 
          "tb_top.tbw" 
          "tb_top.xwv" 
          "tbtpg.tbw" 
          "tbtpg.xwv" 
          "testdaten.sch" 
          "testdaten.sym" 
          "testpattern.sym" 
          "testpattern.vhd" 
          "tgb.sym" 
          "tgb.vhd" 
          "top.sch" 
          "top.sym" 
          "top_guide.ncd" 
                 }
foreach file $export_files {
   CopyOut $file $staging_area $copy_option
}
# copy export file to staging area
CopyOut "D:/borg_sender/sender/sender_import.tcl" "$staging_area" nop
# return back
cd $old_working_dir

