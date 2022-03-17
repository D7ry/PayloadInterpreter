# Payload Interpreter
SKSE plugin acting as an interpreter for Skyrim's animation payload, performing corresponding operations.

**What's a payload?**<br/>
Extract your animation's annotation and open it using a text editor. You can see a bunch of annotations.<br/>
Payloads are essentially the "tail" of annotations.<br/>
For example: <br/>
_SoundPlay.WPNSwingBladeMedium_ is a common annotation with payload. <br/>
"**SoundPlay**" is the annotation, and "**WPNSwingBladeMedium**" is the payload. <br/>

**What does the framework do?**<br/>
Most annotations are not meant to be used with payloads. For example, "hitFrame" and "WeaponSwing" annotations never have payloads. <br/>
However, this does not mean you cannot attach payloads to them. If you attach a payload to "hitFrame", and make it <br/>

_hitFrame.thisIsMyPayload_<br/>

Skyrim can recognize the payload you have just attached, **thisIsMyPayload**. However, at the point, Skyrim will do nothing, because the payload you have attached is just a meaningless sentence.<br/>

Payload interpreter does allows you to pass instructions as payloads which are attached to annotations. The instructions will be called at the exact time the annotation is triggered.<br/>

For example: if I want to cast a fireball when swinging a weapon, I can add @CAST instruction to weaponSwing annotation. I also want that to consume 20 magicka. The result would look like: <br/>
weaponSwing.@CAST|0x002b62|Skyrim.esm|1|1|0|0|0|0|0|20|20


# Common Methods(all begin with "@"):<br/>
set graph variable bool<br/>
@SGVB|_string of graph variable_|0(False)/1(True)<br/>
set graph variable float<br/>
@SGVF|_string of graph variable_|_any float value_<br/>
set graph variable int<br/>
@SGVI|_string of graph variable_|_any int value_<br/>
cast a spell<br/>
@CAST|spell formID|plugin name|effectiveness|magnitude|targetSelf(0/1)|Health Requirement|Health Cost|Stamina Requirement|Stamina Cost|Magicka Requirement|Magicka Cost<br/>

(unimplemented)For actor values:<br/>
@SAV|actor value(in the form of int)|(magnitude as float) - set actor value<br/>
@MAV|av(in the form of int)|-+(magnitude as float) - modify actor value<br/>


