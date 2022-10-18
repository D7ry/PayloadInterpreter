# Payload Interpreter
SKSE plugin acting as an interpreter for Skyrim's animation payload, performing corresponding operations.
https://www.nexusmods.com/skyrimspecialedition/mods/65089
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

In this case, the payload is `@CAST|0x12FD0|Skyrim.esm|1|1|0|0|0|0|0|20|20` <br/>

The resulting annotation looks like: <br/>

`weaponSwing.@CAST|0x12FD0|Skyrim.esm|1|1|0|0|0|0|0|20|20` <br/>

This means: at the precise moment when the "weaponSwing" event triggers, I will cast a spell with formID 0x12FD0(firebolt) stored in Skyrim.esm. The spell has both effectiveness and magnitude being 1, and requires 20 magicka to cast, and also costs 20 magicka.

# Instructions
## Rules
- all payloads are connected to annotations with a `.`
- all payload instructions has a preceding `@`
- parameters are separated by `|`
- It does not matter whether the last parameter has a closing `|`
  - However, it is suggested to not have the closing `|` for stylistic purpose
- Boolean values(True/False) corresponds to `0` and `1`; True = `1`, False = `0`. 
- Passing in true/false as string for boolean values **will not work**
- Negative values can be passed in. Simply add `-` in front of any value
- Quotation marks are not needed when passing in strings as parameters
  - For example, when trying to cast a spell from `Apocalypse.esp`: <br/>
   `@CASTSPELL|0X001|Apocalypse.esp|....` is correct <br/>
   `@CASTSPELL|0X001|"Apocalypse.esp"|....` is incorrect <br/>
- Instructions are case sensitive
- Any numerical parameter not specified are treated as float.


## Dummy event
You have the freedom to attach a payload instruction to any valid animation event. However, you might want to trigger something at a timestamp where there is no animation event to attach payload to. <br/>

Payload Interpreter comes with a nemesis behavior patch that installs a dummy animation event into Skyrim you can safely use. This allows you to call payload instructions freely at any time stamp of any animation, by first adding the dummy animation event, then attaching the payload instruction to the dummy event.

The dummy event is `PIE`. For example, `PIE.@CAST|...` is a valid instruction.

Unlike other events(e.g. weaponSwing, hitFrame), `PIE` itself does absolutely nothing. It exists solely to be a host of payload instructions. You can have as many `PIE` in a single animation as you like.

## List of instructions
- set animation variable bool<br/>
  - `@SGVB|(string)graph variable|(bool)value`<br/>
- set animation variable float<br/>
  - `@SGVF|(string)graph variable|(float)value`<br/>
- set animation variable int<br/>
  - `@SGVI|(string)graph variable|(int)value `<br/>
- cast a spell<br/>
  - `@CASTSPELL|(string)spell formID|(string).esp/.esm/.esl containing the spell|(float)effectiveness|(float)magnitude|(bool)self-targeting|(float)Health Requirement|(float)Health Cost|(float)Stamina Requirement|(float)Stamina Cost|(float)Magicka Requirement|(float)Magicka Cost`<br/>
  - `@CAST|...` can also be used
- Apply a spell and its magic effect onto the actor instantly. Spell may stay on actor.
  - `@APPLYSPELL|(string)spell formID|(string).esp/.esm/.esl containing the spell`
- Remove a spell's effect from the actor.
  - `@UNAPPLYSPELL|(string) spell formID|(string.esp/.esm/.esl containing the spell`
- set the actor to ghost(invincible). If the argument is false, "unghost" the character, and vice versa. <br/>
  - `@SETGHOST|(bool)isghost`<br/>
  - when in ghost state, the character will not get hit by anything(weapon&spell), but can hit others. 
- play a nif particle effect on the actor
  - `@PLAYPARTICLE|(string).nif file's relative path|(int)body part index|(float)nif scale|(float)play time|(utin32) flags|(float)X offset|(float)Y offset|(float)Z offset`
  - works only for bipeds currently
  - nif's path is relative to `meshes` folder, and subdirectories are to be separated with `/`. If my .nif file is in stored in `meshes/ValhallaCombat` and is named `block_Counter.nif`, the argument I want to pass in is `ValhallaCombat/block_Counter.nif`
  - list of body part indices(for none-weapon indices, some are incorrect. For example kHands actually points to head. I encourage you to try with weapon indices(numbers bigger than 33))
    ```
    kHead = 0,
    kHair = 1,
    kBody = 2,
    kHands = 3,
    kForearms = 4,
    kAmulet = 5,
    kRing = 6,
    kFeet = 7,
    kCalves = 8,
    kShield = 9,
    kTail = 10,
    kLongHair = 11,
    kCirclet = 12,
    kEars = 13,
    kDecapitateHead = 20,
    kDecapitate = 21,
    kFX01 = 31,

    kEditorTotal = 32,

    kHandToHandMelee = kEditorTotal,
    kOneHandSword = 33,
    kOneHandDagger = 34,
    kOneHandAxe = 35,
    kOneHandMace = 36,
    kTwoHandMelee = 37,
    kBow = 38,
    kStaff = 39,
    kCrossbow = 40,
    kQuiver = 41,

    kTotal = 42
    ```
I'm working on adding many more methods. Don't hesitate to let me know if you want to do anything specific through payload.

## User-Defined instructions
Having many payload instructions can clutter the annotation and making things difficult to manage. Instead of putting native instructions directly in the annotation, you can define your own payload instruction that maps to one or multiple native instructions, through an .ini file. This way, you can manage and modify specific parameters in the .ini file, without the need to re-annotate.<br/>

- All custom instructions must begin with `$`
- A custom instruction can be mapped to multiple instructions. Example below.
- .ini file should be stored in SKSE/PayloadInterpreter/Config
- .ini file must have at least one section. Example below.
- Do not include user-defined instruction in itself. Doing so would lead to a recursive loop that will set your pc on fire.

### Example:
```
[WeaponArt]
$shieldCharge = @PLAYPARTICLE|ValhallaCombat/impactShieldRoot.nif|3|1|0|4U|0|50|50
$shieldCharge = @CAMSHAKE|1|0.5
$shieldCharge = @CAST|0x12FD0|Skyrim.esm|1|1|0|0|0|0|0|20|20

$swordFlame = @CAST|0x12FD0|Skyrim.esm|1|1|0|0|0|0|0|20|20
$swordFlame = @CAMSHAKE|2|5
$swordFlame = @PLAYPARTICLE|ValhallaCombat/impactSwordRoot.nif|3|1|0|4U|0|50|50

[Convenience]
$enableIframe = @SETGHOST|1
$disableIframe = @SETGHOST|0
```
- The above case consists of two sections: `WeaponArt` and `Convenience`. There are no specific rule for section grouping/naming, organize as you wish, but make sure to have at least one section.
- In `WeaponArt` section, `$swordFlame` is a custom instruction mapped to three native instructions:`@CAST`, `@CAMSHAKE`, and `@PLAYPARTICLE`
  - In other words, if you attach `$swordFlame` to `weaponSwing`, when you swing your weapon, your camera shakes, a particle effect is played in front of you, and you cast a firebolt spell.
  - Because the parameters of those instructions are written in an .ini file, you can easily modify the spell costs or the .nif file being played, without re-annotating.
- In `Convenience` section, `$enableIframe` is mapped to one native instruction `@SETGHOST|1`. While it seems unncessary as they're about the same length, this custom payload allows for better clarity.

## Asynchronous operations
Sometimes you might want to do something a little bit after the current payload time(e.g. disabling i-frame, remove actor buff/ change actor values). This can be done using asynchronous instructions. Asynchronous instructions are almost identical to native or user defined instructions, except they contain an additional argument: time.

Example of an asynchronous operation:<br/>
``![5]@SETGHOST|0``<br/>
What makes it different, is the set of `[]`, and a `!` in the front. The leading `!` signifies it being an asynchronous operation, and `5` in the first bracket corresponds to the wait time (before the instruction fires). Nothing following needs to be changed.

- Asynchronous operations must begin with `!`
- They also must contain a set of brackets `[]` immediately following `!`
 - The brackets contains the wait time of this instruction.
 - immediately following the brackets is the actual instruction
- You can make either user-defined or native asynchronous. You can even have asynchrnous instructions inside user-defined instructions.
- For example: ``![5]@SETGHOST|0`` translates to "unghost me 5 seconds later".
## DEBUG
If you think you did everything right and nothing shows up in your game, you can look up the plugin log from `c\users\yourUserName\Documents\My Games\Skyrim Special Edition\SKSE\PayloadFramework.log`. The log will prints out precisely the errored payload.
