# Payload Interpreter
SKSE plugin acting as an interpreter for Skyrim's animation payload, performing corresponding operations.

## What is payload?
Extract your animation's annotation and open it using a text editor. You can see a bunch of annotations.<br/>
Payloads are the "tail" of annotations, attached to the annotation body, separated by a `.`<br/>
For example: <br/>
`SoundPlay.WPNSwingBladeMedium` is a common annotation with payload. <br/>
`SoundPlay` being the annotation, and `WPNSwingBladeMedium` being the payload. <br/>

## What does the interpreter do?
Most annotations are not meant to be used with payloads. For example, `hitFrame` and `WeaponSwing` annotations never have payloads attached to them. <br/>

This does not mean you cannot attach payloads to them. For example, when you attach a payload `aPayLoad` to `hitFrame` the resulting text looks like: <br/>

`hitFrame.aPayLoad`

In game, when you play the animation with this modified annotation, at the precise moment when `hitFrame.aPayLoad` is triggered, Skyrim can recognize this payload `aPayLoad`. However, nothing would happen at all, since `aPayLoad` is a meaningless string. 

**Payload Interpreter looks at the (meaningless)strings passed in, and translate valid strings into instructions.** The instructions will be called on at the exact moment the payload's annotation is triggered. In other words, it works just like python, but for Skyrim animations. <br/>

## Examples
Here's an example. I want to cast a fireball spell when swinging a weapon. <br/>

First, look for the annotation that corresponds to the moment I want the cast to happen. <br/>

In this case, the desired annotation is `weaponSwing` in my attack animation. <br/>

Then, attach the payload instruction and its parameters onto that annotation. <br/>

In this case, the payload is `@CAST|0x002b62|Skyrim.esm|1|1|0|0|0|0|0|20|20` <br/>

The resulting annotation looks like: <br/>

`weaponSwing.@CAST|0x002b62|Skyrim.esm|1|1|0|0|0|0|0|20|20` <br/>

This means: at the precise moment when the "weaponSwing" event triggers, I will cast a spell with formID 0x002b62 stored in Skyrim.esm. The spell has both effectiveness and magnitude being 1, and requires 20 magicka to cast, and also costs 20 magicka.

# List of Instructions
## Rules
- all payloads are connected to annotations with a `.`
- all payload instructions has a preceding `@`
- parameters are separated by `|`
- It does not matter whether the last parameter has a closing `|`
  - However, it is suggested to not have the closing `|` for stylistic purpose
- Boolean values(True/False) corresponds to `0` and `1`; True = `1`, False = `0`. 
- Passing in true/false as string for boolean values **will not work**
- Negative values can be passed in. Simply add `-` in front of any value
- Quotes are not needed when passing in strings as parameters
  - For example, when trying to cast a spell from `Apocalypse.esp`: <br/>
   `@CAST|0X001|Apocalypse.esp|....` is correct <br/>
   `@CAST|0X001|"Apocalypse.esp"|....` is incorrect <br/>
- Instructions are case sensitive
- Any numerical parameter not specified are treated as float.


## Dummy event
You have the freedom to attach a payload instruction to any valid animation event. However, you might want to trigger something at a timestamp where there is no animation event to attach payload to. <br/>
Payload Interpreter comes with a nemesis behavior patch that installs a dummy animation event into Skyrim that you can safely use. This allows you to call payload instructions freely at any time stamp of any animation, by first adding the dummy animation event, then attaching the payload instruction to the dummy event.

The dummy event is `PIE`. For example, `PIE.@CAST|...` is a valid instruction.

Unlike other events(e.g. weaponSwing, hitFrame), `PIE` itself does absolutely nothing. It exists solely to be a host of payload instructions. You can have as many `PIE` in a single animation as you like.

## Instructions
set animation variable bool<br/>
`@SGVB|(string)graph variable|(bool)value`<br/>
set animation variable float<br/>
`@SGVF|(string)graph variable|(float)value`<br/>
set animation variable int<br/>
`@SGVI|(string)graph variable|(int)value `<br/>
cast a spell<br/>
`@CAST|(string)spell formID|(string).esp/.esm/.esl containing the spell|(float)effectiveness|(float)magnitude|(bool)self-targeting|(float)Health Requirement|(float)Health Cost|(float)Stamina Requirement|(float)Stamina Cost|(float)Magicka Requirement|(float)Magicka Cost`<br/>
set the actor to ghost(invincible). If the argument is false, "unghost" the character, and vice versa. <br/>
`@SETGHOST|(bool)isghost`<br/>

I'm working on more methods. Don't hesitate to let me know if you want to do anything specific through payload.

