## 프로젝트 개요

DB 연결 테스트용 더미 데이터를 생성·삽입하는 콘솔 PoC 도구. 사용자가 숫자를 입력하면 해당 수만큼 더미 레코드를 생성하고 DB에 추가한다. JSON 파일 설정을 통해 기본값을 제공하며, 콘솔 인터페이스는 한글 출력을 지원한다.

**DB는 JSON 파일로 구현한다.** `data/records.json`이 데이터 저장소 역할을 하며, 별도의 RDBMS나 외부 DB 연결 없이 nlohmann/json을 통해 읽고 쓴다.

기본 레코드 스키마: `{ "id": int, "name": string, "value": string }`

## 빌드

```powershell
# Debug x64 빌드
msbuild DummyDataGenerator-KHNam-CRA-3.vcxproj /p:Configuration=Debug /p:Platform=x64

# Release x64 빌드
msbuild DummyDataGenerator-KHNam-CRA-3.vcxproj /p:Configuration=Release /p:Platform=x64
```

- Toolset: v145 (VS 2022), C++20, Windows SDK 10.0+, Console subsystem
- 한글 소스 파일 사용 시 `/utf-8` 컴파일러 옵션을 `.vcxproj`의 `<AdditionalOptions>`에 추가해야 한다.
- 소스 파일을 추가할 때마다 `.vcxproj`의 `<ClCompile>` / `<ClInclude>` 항목도 함께 갱신해야 한다.

## 아키텍처

이 저장소의 형제 프로젝트(DataPersistence-KHNam-CRA-3, ConsoleMVC-KHNam-CRA-3)와 동일한 구조를 따른다.

```
src/
├── main.cpp          # 콘솔 메뉴 루프, 진입점
├── Model.h           # Record 구조체 정의
├── Repository.h/.cpp # JSON 파일 I/O (DB 역할)
└── Generator.h/.cpp  # 더미 데이터 생성 로직

data/
├── default.json      # 기본 레코드 설정값 (id/name/value 3개 필드)
└── records.json      # 생성된 더미 데이터가 저장되는 파일 (DB)

vendor/
└── json.hpp          # nlohmann/json v3.11.3 (단일 헤더)
```

### JSON 파일 = DB

`data/records.json`이 DB 역할을 한다. Repository가 시작 시 파일을 메모리(`std::vector<Record>`)로 로드하고, 더미 데이터 삽입 후 전체를 파일에 다시 쓴다.

```cpp
#include "vendor/json.hpp"
using json = nlohmann::json;

// 읽기
json arr = json::parse(file);
// 쓰기 (2-space indent)
file << arr.dump(2) << '\n';
```

### 한글 콘솔 I/O

`main()` 최상단에서 코드페이지를 설정한다.

```cpp
#include <windows.h>
SetConsoleCP(CP_UTF8);
SetConsoleOutputCP(CP_UTF8);
```

### 콘솔 인터페이스 패턴

숫자 입력 → 해당 수만큼 더미 레코드 생성 → `data/records.json`에 추가. 잘못된 입력 시 한글 오류 메시지를 출력하고 재입력을 받는다.

```cpp
int count;
std::cin >> count;
// Generator::generate(count, defaultConfig) → Repository::insertAll(records)
```
