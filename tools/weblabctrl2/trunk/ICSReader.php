<?php
/**
 * A very simple unassuming ICS parser.
 *
 * Reads a ICS file and splits the information into useable arrays.
 * Does not try and interpret the specification, just provides a raw data interface.
 *
 * @author      Aidan Lister <aidan@php.net>
 * @version     1.0.0
 * @link        http://aidanlister.com/repos/v/iCalReader.php
 */


// class ICSutil 
// {
//   static private $sortfield = null;
//   static private $sortorder = 1;
//   static private function sort_callback(&$a, &$b) 
//   {
//     if($a[self->$$sortfield] == $b[self->$$sortfield]) return 0;
//     return ($a[self::$sortfield] < $b[self::$sortfield])? -self::$sortorder : self::$sortorder;
//   }
//   static function sort(&$v, $field, $asc=true) 
//   {
//     self::$sortfield = $field;
//     self::$sortorder = $asc? 1 : -1;
//     usort($v, array('ICSutil', 'sort_callback'));
//   }
// }

class ICSEvent
{
  /*
BEGIN:VEVENT
DTSTART;VALUE=DATE:20100311
DTEND;VALUE=DATE:20100311
SUMMARY:REMONDIS DSD, Abholung am Folgetag: gelbe Tonnen (grosse und kleine
 ) zur Strasse!
END:VEVENT
  */
  var $_start_date=0;
  var $_end_date=0;
  var $_summary=0;

  function sort_callback($a,$b)
  {
    if ( $a->get_start_date() == $b->get_start_date() ) {
      return 0;
    }
    if ( $a->get_start_date() < $b->get_start_date() ) {
      return -1;
    }
    return 1;
  }

  function ICSEvent($icsStrings)
  {
    $this->parse_event($icsStrings);
  }

  function parse_event($icsStrings)
  {
    $in_summary = false;
#    var_dump($icsStrings);
    foreach($icsStrings as $line)
      {
	if(preg_match('/^BEGIN:VEVENT/',$line))
	  {
	    $in_summary = false;
	    continue;
	  }
	if(preg_match('/^END:VEVENT/',$line))
	  {
	    $in_summary = false;
	    continue;
	  }
	if(preg_match('/^DTSTART;VALUE=DATE:/',$line))
	  {
	    $this->set_start_date($line);
	    $in_summary = false;
	    continue;
	  }
	if(preg_match('/^DTEND;VALUE=DATE:/',$line))
	  {
	    $this->set_end_date(trim($line));
	    $in_summary = false;
	    continue;
	  }
	if(preg_match('/^SUMMARY:/',$line))
	  {
	    $this->set_summary($line);
	    $in_summary = true;
	    continue;
	  }
	if($in_summary==true){
	  $this->append_summary($line);
	  continue;
	}
	if(preg_match('/^RRULE:/',$line))
	  {
	    
	    continue;
	  }
      }
  }
  

  function append_summary($summary)
  {
    $this->_summary.=trim($summary);
  }

  function set_summary($summary)
  {
    $tmp=array();
    $tmp=split(':',$summary,2);
    $this->_summary=trim($tmp[1]);
  }
  function set_start_date($start_date)
  {
    $start_date = explode( ':',$start_date);
    $dt=trim($start_date[1]);
    $hour=0;
    $minute=0;
    $second=0;
    $month=substr($dt,4,2);
    $day=substr($dt,6,2);
    $year=substr($dt,0,4);
#    $format="Ymd";
 #   $dt = DateTime::createFromFormat($format, $start_date[1]);
    $dt=mktime($hour,$minute,$second,$month,$day,$year);

    $this->_start_date=$dt;
  }
  function set_end_date($end_date)
  {
    $end_date = explode( ':',$end_date);
    $dt=trim($end_date[1]);
    $hour=0;
    $minute=0;
    $second=0;
    $month=substr($dt,4,2);
    $day=substr($dt,6,2);
    $year=substr($dt,0,4);
#    $format="Ymd";
 #   $dt = DateTime::createFromFormat($format, $start_date[1]);
    $dt=mktime($hour,$minute,$second,$month,$day,$year);
    $this->_end_date=$dt;
  }

  function get_summary()
  {
    return $this->_summary;
  }
  function get_start_date()
  {
    return $this->_start_date;
  }
  function get_end_date()
  {
    return $this->_end_date;
  }

}

class ICSReader
{
  /**
   * Element array of data source
   *
   * @var     array
   * @access  private
   */
  var $_source = '';

  /**
   * Line number to be parsed next
   *
   * @var     int
   * @access  private
   */
  var $_linenum = 0;

  /**
   * Parsed data
   *
   * @var     array
   * @access  private
   */
  var $_data = array();

  var $_relevant_tokens = array();
  /**
   * Constructor
   *
   * @param   string  Path to file to be parsed
   * @access  public
   */


  function ICSReader()
  {

  }

  function readfromfile($filename)
  {
    $this->_source = file_get_contents($filename);
    $this->_source = explode("\n",$this->_source);
    $this->_parseEvents();
  }

  /**
   * Return parsed event information
   *
   * @access  public
   * @return  array   Parsed event information
   */
  function getEvents()
  {
    usort($this->_data, array("ICSEvent",'sort_callback'));

    return $this->_data;
  }

  function _find_first_event_start()
  {
    return $this->_find_next_event_start(0);
  }

  function _find_next_event_start($start_line)
  {
    $i=$start_line;
    $runcounter=0;
#    var_dump($this->_source);
    while(isset($this->_source[$i]))
      {
	if ($runcounter > 100){ die("find_next $start_line $i $runcounter");}
	if(preg_match('/^BEGIN:VEVENT/',$this->_source[$i]))
	  {
	    return $i;
	  }
	$i++;
	$runcounter++;
      }
    return $start_line;
  }

  function _find_current_event_end($start_line)
  {
    $i=$start_line;
    $runcounter=0;
    while(isset($this->_source[$i]))
      {
	if ($runcounter > 100){ die("find_end");}
	if(preg_match('/^END:VEVENT/',$this->_source[$i]))
	  return $i;
	$i++;
	$runcounter++;
      }
    return $start_line;
  }


  /**
   * Parse events information
   *
   * @access  private
   */
  function _parseEvents()
  {
    // Init
    $eventstring = array();
    $event_start = $this->_find_first_event_start();
    $event_end = $this->_find_current_event_end($event_start);

    $i=0;
    for ($i=$event_start;$i<=$event_end;$i++)
      {
	array_push($eventstring,$this->_source[$i]);
      }
    array_push($this->_data,new ICSEvent($eventstring));

    $line=$event_end+1;

    // Iterate source
    while (!preg_match('/^END:VCALENDAR/',$this->_source[$line])) {
      if ($line > 2000){ die();}
      $eventstring = array();
      $event_start = $this->_find_next_event_start($event_end);
      $event_end = $this->_find_current_event_end($event_start);

      $i=0;
      for ($i=$event_start;$i<=$event_end;$i++)
	{
	  array_push($eventstring,$this->_source[$i]);
	}
      array_push($this->_data,new ICSEvent($eventstring));

      $line=$event_end+1;
    }

  }

}
?>