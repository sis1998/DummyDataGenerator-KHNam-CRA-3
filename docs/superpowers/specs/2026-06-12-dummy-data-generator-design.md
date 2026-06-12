# DummyDataGenerator 설계 문서

## 개요

DB 연결 테스트용 더미 데이터를 생성·삽입하는 콘솔 PoC 도구.
사용자가 숫자를 입력하면 해당 수만큼 더미 레코드를 생성해 JSON 파일(DB 역할)에 저장한다.
레코드 스키마는 `default.json`으로 정의하며, 필드를 추가·제거하면 생성 결과도 그대로 반영된다.

---

## 아키텍처

### 컴포넌트

| 클래스 | 파일 | 역할 |
|---|---|---|
| `FieldDef` / `Record` | `src/Model.h` | 필드 정의 구조체, `Record = map<string,string>` 타입 alias |
| `ConfigLoader` | `src/ConfigLoader.h/.cpp` | `default.json` 파싱 → `vector<FieldDef>` 반환 |
| `Generator` | `src/Generator.h/.cpp` | `FieldDef` 목록 기반으로 n개 Record 생성 |
| `Repository` | `src/Repository.h/.cpp` | `records.json` 로드·저장, 레코드 추가 |
| `ConsoleUI` | `src/ConsoleUI.h/.cpp` | 메뉴 루프, 입력 파싱, 한글 출력 |
| `main` | `src/main.cpp` | 코드페이지 설정, 객체 연결, 루프 시작 |

### 의존 관계

```
main
 ├─ ConfigLoader  →  Model
 ├─ Repository    →  Model
 ├─ Generator     →  Model
 └─ ConsoleUI     →  Generator, Repository
```

---

## 데이터 구조

### `default.json` (필드 스키마 정의)

```json
{
  "fields": [
    { "name": "id",    "type": "int",    "prefix": "" },
    { "name": "name",  "type": "string", "prefix": "name-" },
    { "name": "value", "type": "string", "prefix": "value-" }
  ]
}
```

- `prefix`가 빈 문자열이면 인덱스 숫자만 값으로 사용 (id 필드)
- 필드 순서가 JSON 출력 순서를 결정
- `type` 필드는 현재 모든 값을 문자열로 저장하며, 향후 유효성 검사나 타입 변환 확장을 위한 메타데이터로만 사용

### `records.json` (생성 결과 저장)

```json
[
  { "id": "1", "name": "name-1", "value": "value-1" },
  { "id": "2", "name": "name-2", "value": "value-2" }
]
```

### Model 타입

```cpp
struct FieldDef {
    std::string name;
    std::string type;    // "int" | "string"
    std::string prefix;
};

using Record = std::map<std::string, std::string>;
```

---

## 데이터 흐름

### 시작 시

```
main()
 ├─ SetConsoleCP(CP_UTF8) / SetConsoleOutputCP(CP_UTF8)
 ├─ ConfigLoader::load("data/default.json") → vector<FieldDef>
 ├─ Repository::load("data/records.json")   → vector<Record>
 │    └─ 기존 데이터 존재 시 → ConsoleUI: "추가/초기화" 선택 요청
 └─ ConsoleUI::run(fieldDefs, repository)   → 메뉴 루프 진입
```

### 메뉴 루프

```
[메뉴]
  1. 더미 데이터 생성
  0. 종료

1 선택:
  ConsoleUI → "생성할 수를 입력하세요" → n
  startId   = 현재 records의 마지막 id + 1  (초기화 모드면 1)
  Generator::generate(n, fieldDefs, startId) → vector<Record>
  Repository::append(records) → records.json 저장
  ConsoleUI → "N건 생성 완료" 출력
  → 메뉴 복귀

0 입력:
  프로그램 종료
```

### ID 증가 규칙

- **추가 모드**: 기존 레코드의 마지막 id + 1 부터 채번
- **초기화 모드**: 1 부터 시작
- `id` 필드의 `prefix`가 `""` 이므로 값은 순수 숫자 문자열 (`"1"`, `"2"`, ...)

---

## 오류 처리

| 상황 | 처리 |
|---|---|
| `default.json` 없음 | 오류 메시지 출력 후 종료 (필수 파일) |
| `records.json` 없음 | 정상 — 빈 상태로 시작, 저장 시 신규 생성 |
| 잘못된 입력 (비숫자) | 한글 오류 메시지 후 재입력 |
| 0 이하 입력 | "1 이상의 숫자를 입력하세요" 후 재입력 |
| JSON 파싱 실패 | 오류 메시지 출력 후 종료 |
| 파일 쓰기 실패 | 오류 메시지 출력 (데이터 유실 경고 포함) |

내부 함수는 `std::exception`을 throw하고, `main()`에서 catch해 한글 메시지로 출력한다.

---

## 파일 구조

```
src/
├── main.cpp
├── Model.h
├── ConfigLoader.h / ConfigLoader.cpp
├── Generator.h   / Generator.cpp
├── Repository.h  / Repository.cpp
└── ConsoleUI.h   / ConsoleUI.cpp

data/
├── default.json      # 필드 스키마 정의 (필수)
└── records.json      # 생성된 더미 데이터 (자동 생성)

vendor/
└── json.hpp          # nlohmann/json v3.11.3
```
