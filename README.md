# :computer: P_Ashen_Crown
<div align="center">
<img src="https://github.com/user-attachments/assets/6240dab0-e934-4015-9dbf-5418d651afe5" width="600" height="450"/>
</div>

This project is a medieval-themed Soulslike game developed using Unreal Engine 5.5. It is an alpha project with a playtime of approximately 5 minutes. As a team project, I was responsible for developing the UI, cinematics, and lock-on system.

이 프로젝트는 Unreal Engine 5.5를 사용하여 개발한 중세 배경의 소울라이크 게임입니다. 플레이 타임은 5분 가량의 알파 프로젝트입니다. 팀원으로서 UI와 시네마틱, 락온 시스템을 개발을 맡았습니다.

## :pushpin: Main Architecture (핵심 아키텍처)
<div align="center">
<img src="https://github.com/user-attachments/assets/639dcdbe-81ea-47d6-998f-320ce1350205" width="600" height="450"/>
</div>
<div align="center">
<img src="https://github.com/user-attachments/assets/5b008bf4-cdda-440e-b156-c8f06917457d" width="600" height="450"/>
</div>

The Dialogue Component is a system that constructs conversational flows based on dialogue data defined in a DataTable. The overall design is inspired by the AI Behavior Tree structure in Unreal Engine, aiming to create a data-driven dialogue system that is both highly extensible and maintainable.
* Each conversation is organized into paragraphs, and Tasks can be assigned to each paragraph to trigger events at the moment of transition between paragraphs.
* Choices can have Decorators applied to control their availability based on specific conditions. This allows for branching dialogue that adapts to the player’s state or progression.
* Tasks and Decorators are not instantiated repeatedly. Instead, they are executed in a reusable form through Data Assets and Class Default Object (CDO). This approach reduces unnecessary object creation and contributes to overall performance optimization. 

Dialogue Component는 주어진 DataTable에 정의된 대사 데이터를 기반으로 대화 흐름을 구성하는 컴포넌트입니다. 전체 설계는 언리얼 엔진의 AI Behavior Tree 구조를 참고하였으며, 데이터 기반이면서도 확장성과 유지보수성이 높은 대화 시스템을 지향하여 제작되었습니다. 
* 각 대화는 문단 단위로 구성되며, 문단마다 Task를 배치하여 문단 전환 시점에 이벤트를 실행할 수 있습니다.
* 선택지에는 Decorator를 적용하여 조건에 따라 활성화 여부를 제어합니다. 이를 통해 플레이어의 상태나 진행 상황에 따라 분기되는 대화를 구현할 수 있습니다.
* Task와 Decorator는 매번 인스턴싱하지 않고, Data Asset과 CDO(Class Default Object)를 통해 재사용 가능한 형태로 실행됩니다. 이러한 불필요한 객체 생성을 줄여 최적화하는 데 기여합니다.

## :wrench: Tools & Technologies (사용한 기술)
- **Client** : Unreal Engine 5.5
- **Plugin** : View Model, Gameplay Ability System

## :octocat: Git Commit Comment Rule (커밋 규칙)
```
type: Title

body(Optional)
```
```
Types
+ feat : 기능 추가
+ fix : 오류 수정
+ docs : 문서 추가
```

+ 한글로 작성
+ 무엇을 추가했고 왜 추가했는지 요약

## :open_file_folder: Project Folder Rules (프로젝트 폴더 규칙)
```
Contents/AC : Project Folder
Contents/AC/SourceArt
Contents/AC/SourceArt/SkletalMesh
Contents/AC/SourceArt/StaticMesh
Contents/AC/SourceArt/Animation
Contents/AC/SourceArt/Texture
Contents/AC/SourceArt/Material
Contents/AC/SourceArt/VFX
Contents/AC/SourceArt/Media
Contents/AC/SourceArt/Sound
Contents/AC/Map
Contents/AC/Data : Curve, DataTable, Parameter Folder
Contents/AC/Blueprint
Contents/AC/Blueprint/AI : Behabior Tree, BlackBoard, EQS
Contents/AC/Blueprint/Anim : Animation BP, AnimMontage
```

## :pencil: Asset Naming Rules (에셋 이름 규칙)
```
Prefix_BaseAssetName_Variant_SuffixPermalink
ex)
MainCharacter's 3rd Skeletion's Normal Texture
== T_MainCharSkeltion_03_N
```
+ Asset의 Prefix는 단어의 첫 알파벳들을 사용 (ex. Material Instance == MI)
+ Variant가 없는 경우에는 00을 사용
+ SuffixPermalink는 부가적 설명란으로 텍스처의 경우 노말인지, 오클루전인지 등을 표현 (ex. Normal == N) 

## :pencil: Object Naming Rules (객체 이름 규칙)
```
BP_ : 블루프린트
E : Unreal c++ && BP 열거형 Prefix (※ c++에서는 자동으로 붙음)
F : Unreal c++ && BP 구조체 Prefix (※ c++에서는 자동으로 붙음)
FOn"Verb" : 대리자 Prefix
b : bool Prefix (ex. bIsCompleted, bIsDead)
m : member 변수 Prefix
```

## :pencil: Code Comment Rules (주석 규칙)
```
/// <summary>
/// 함수 구현 설명
/// </summary>
/// <param name="파라미터명"> 파라미터 값 설명 </param>
/// <returns> 리턴 값 설명 </returns>

/**
 * 함수 구현 설명
 * @param 파라미터명 파라미터 값 설명
 * @return 리턴 값 설명
 */
```
+ Get/Set 함수은 주석 제외
+ 일반 함수는 위의 둘 중 하나의 방식으로 주석 작성
