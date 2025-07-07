# P_Ashen_Crown
언리얼 엔진 기반 소울라이크 게임 
The soullike project based Unreal Engine.

## Git Commit Comment (깃 커밋 규칙)
```
type: Title

body(Optional)

```
```
Types
+ feat (to add new tool)
+ fix
+ docs
+ style	(to change only code outfit)
+ design (UI)
+ test
+ refactor
+ build
+ ci	(to change config files)
+ perf	(to upgrade performance)
+ rename
+ remove
```

+ Separate title and body with a blank line (/n)
+ Title is used as a statement
+ Describe what you did and why 

## Project Folder Rules
```
Contents : Don't touch
Contents/SB : Project Folder (To prevent folders from merging due to migration)
Contents/SB/SourceArt : AssetFile Folder
Contents/SB/SourceArt/Character : Skeletion Folder
Contents/SB/SourceArt/Animation : Animation Folder
Contents/SB/SourceArt/StaticMesh : StaticMesh Folder
Contents/SB/SourceArt/Art : Texture, Material Folder
Contents/SB/SourceArt/VFX : Niagara, CascadeParticle Folder
Contents/SB/SourceArt/Media : Video Folder
Contents/SB/SourceArt/Sound : BGM, SE Folder
Contents/SB/Map : Map Folder
Contents/SB/Data : Curve, DataTable, Parameter Folder
Contents/SB/Blueprint : Blueprint Source Folder
Contents/SB/Blueprint/AI : Behabior Tree, BlackBoard, EQS, AI Controller BP Folder
Contents/SB/Blueprint/Anim : Animation BP, AnimMontage Folder
```

## Project Name Rules
Asset Name Rule
```
Prefix_BaseAssetName_Variant_SuffixPermalink
ex)
MainCharacter's 3rd Skeletion's Normal Texture
== T_MainCharSkeltion_03_N
```
Code Prefix Name Rule
```
BP_ : Blueprint Prefix
E : Unreal c++ && BP Enum Prefix (※ It is automatically attached in c++)
F : Unreal c++ && BP Struct Prefix (※ It is automatically attached in c++)
```
Code Type Name Rule
```
+ UE value's name follows PascalCase
+ Server c++ value's name follows camelCase

+ Private field prefix is '_'
+ Bool value prefix is 'b'. Make sure the name is true or false 
ex) bIsCompleted, bIsDead
+ Variable names do not contain class names
ex) UEngine::EngineVersion X -> UEngine::Version O
+ Event, Dispatch name is 'On(Action)'

+ Protocol struct and class name follow PascalCase
+ Protocol value's name follows snake_case
```
Code Comment Rule
```
Describe

Parameter
Name (in) : Describe
&Name (out) : Describe

Return : Describe
```
