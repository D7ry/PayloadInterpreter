# PayloadFramework
SKSE plugin acting as an interpreter for Skyrim's animation payload, performing corresponding operations.

Common Methods(all begin with "@"):<br/>
set graph variable bool<br/>
@SGVB|_string of graph variable_|0(False)/1(True)|<br/>
set graph variable float<br/>
@SGVF|_string of graph variable_|_any float value_|<br/>
set graph variable int<br/>
@SGVI|_string of graph variable_|_any int value_|<br/>
cast a spell<br/>
@CAST|spell formID|plugin name|effectiveness|magnitude|targetSelf(0/1)|Health Requirement|Health Cost|Stamina Requirement|Stamina Cost|Magicka Requirement|Magicka Cost<br/>

(unimplemented)For actor values:<br/>
@SAV|actor value(in the form of int)|(magnitude as float)| - set actor value<br/>
@MAV|av(in the form of int)|-+(magnitude as float)| - modify actor value<br/>


