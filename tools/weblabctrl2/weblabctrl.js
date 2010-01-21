function movepic(img_name,img_src) {
  document[img_name].src=img_src;
}

function set_value(cmd,id,value)
{
  new Ajax.Updater('ajax', 'set.php?cmd='+cmd+'&id='+id+'&value='+value,{method:'get', onComplete:function() {done=true;}, evalScripts: true} );
}
function toilet_cmd(id,value)
{
  new Ajax.Updater('ajax', 'set.php?cmd=toilet&id='+id+'&value='+value,{method:'get', onComplete:function() {done=true;}, evalScripts: true} );
}
function treppenblink_cmd(value)
{
  new Ajax.Updater('ajax', 'set.php?cmd=treppenblink&value='+value,{method:'get', onComplete:function() {done=true;}} );
}
function canir_beamer_cmd(value)
{
  new Ajax.Updater('ajax', 'set.php?cmd=canir_beamer&value='+value,{method:'get', onComplete:function() {done=true;}} );
}
function canir_teufel_cmd(value)
{
  new Ajax.Updater('ajax', 'set.php?cmd=canir_teufel&value='+value,{method:'get', onComplete:function() {done=true;}} );
}
function mpd_cmd(room,cmd,value)
{
  new Ajax.Updater('ajax', 'mpd.php?cmd='+cmd+'&room='+room+'&value='+value,{method:'get', onComplete:function() {done=true;}} );
}
function beamer_on()
{
  new Ajax.Updater('ajax', 'set.php?cmd=beamer_on',{method:'get', onComplete:function() {done=true;}} );
}
function coinflip()
{
  new Ajax.Updater('ajax', 'set.php?cmd=coinflip',{method:'get', onComplete:function() {done=true;}} );
}
function dice()
{
  new Ajax.Updater('ajax', 'set.php?cmd=dice',{method:'get', onComplete:function() {done=true;}} );
}
function w20()
{
  new Ajax.Updater('ajax', 'set.php?cmd=w20',{method:'get', onComplete:function() {done=true;}} );
}
function text_the_borg(text)
{
  new Ajax.Updater('ajax', 'set.php?cmd=text_the_borg&text='+text,{method:'get', onComplete:function() {done=true;}} );
}
function save_pos(element)
{
  new Ajax.Updater('ajax', 'set.php?cmd=save_pos&x='+document.getElementById(element.id).style.left+'&y='+document.getElementById(element.id).style.top,{method:'get', onComplete:function() {done=true;}} );
}
function set_bg(whichImage){
  new Ajax.Updater('ajax', 'set.php?cmd=save_bg&bg='+whichImage,{method:'get', onComplete:function() {done=true;}} );
  document.getElementById('body').className=whichImage;

}
function blue_mode(mode)
{
  new Ajax.Updater('ajax', 'set.php?cmd=blue_mode&mode='+mode,{method:'get', onComplete:function() {done=true;}} );
}
function moodbar(r,g,b)
{
  new Ajax.Updater('ajax', 'set.php?cmd=moodbar&r='+r+'&g='+g+'&b='+b,{method:'get', onComplete:function() {done=true;}} );
}
function moodbar_fade()
{
  new Ajax.Updater('ajax', 'set.php?cmd=moodbar_fade',{method:'get', onComplete:function() {done=true;}} );
}


