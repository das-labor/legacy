def lookup_smile(text)
  text = textilize html_escape(text)
  z_angry = [ ":-@", ":@" ]
  z_confused = [ ":-S", ":S", ":-s", ":s" ]
  z_cry = [ ":'(", ":'-(", ";-(", ";(" ]
  z_embarassed = [ ":-$", ":$", ":-x", ":x", ":-X", ":X" ]
  z_lightbulb = [ "(i)", "(I)" ]
  z_love = [ "(l)", "(L)" ]
  z_omg = [ ":-o", ":o", ":-O", ":O" ]
  z_sad = [ ":-(", ":(" ]
  z_shade = [ "8-)", "8)" ]
  z_smile = [ ":-)", ":)", ":o)"]
  z_star = [ "(*)" ]
  z_teeth = [ ":d", ":-d", ":D", ":-D" ]
  z_thumbs_down = [ "(n)", "(N)" ]
  z_thumbs_up = [ "(y)", "(Y)" ]
  z_tongue = [ ":-p", ":p", ":-P", ":P" ]
  z_unlove = [ "(u)", "(U)" ]
  z_wink = [ ";-)", ";)" ]
  dispozitie = { z_angry => 'angry.png', 
                 z_confused => 'confused.png', 
                 z_cry => 'cry.png',
                 z_embarassed => 'embarassed.png',
                 z_lightbulb => 'lightbulb.png',
                 z_love => 'love.png',
                 z_omg => 'omg.png',
                 z_sad => 'sad.png',
                 z_shade => 'shade.png',
                 z_smile => 'smile.png',
                 z_star => 'star.png',
                 z_teeth => 'teeth.png',
                 z_thumbs_up => 'thumbs_up.png',
                 z_thumbs_down => 'thumbs_down.png',
                 z_tongue => 'tongue.png',
                 z_unlove => 'unlove.png',
                 z_wink => 'wink.png' }
  dispozitie.each do | zambete, poze_zambete |
    zambete.each { |zambet| text.gsub!(zambet, "<img src ='/images/zambete/#{poze_zambete}' />") }
  end
  
  return text
end