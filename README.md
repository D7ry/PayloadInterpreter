# PayloadFramework
SKSE plugin acting as an interpreter for Skyrim's animation payload, performing corresponding operations.

Common Methods(all begin with "#"):

For graph variables:<br/>
#SGVB|"string of graph variable"|0(False)/1(True)| - set graph variable bool<br/>
#SGVF|"string of graph variable"|any float value| - set graph variable float<br/>
#SGVI|"string of graph variable"|any int value| - set graph variable int<br/>
#MGVF|"string of graph variable"|any float value| - modify graph variable float<br/>
#MGVI|"string of graph variable"|any int value|- modify graph variable int<br/>

For actor values:<br/>
#SAV|actor value(in the form of int)|(magnitude as float)| - set actor value<br/>
#MAV|av(in the form of int)|-+(magnitude as float)| - modify actor value<br/>

#CAST|spell form|plugin name|effectiveness|magnitude|targetSelf(0/1)|magickaCost|StaminaCost| - cast a spell<br/>

Convenience Methods(all begin with "$"):<br/>
$iFrameOn - enable iFrame<br/>
$iFrameOff - disbale iFrame<br/>
