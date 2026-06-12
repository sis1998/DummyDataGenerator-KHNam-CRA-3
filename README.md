# DummyDataGenerator

DB 연결 테스트용 더미 데이터를 생성·저장하는 콘솔 PoC 도구.
숫자를 입력하면 해당 수만큼 더미 레코드를 생성해 JSON 파일에 추가한다.
레코드 스키마는 `data/default.json`으로 자유롭게 정의할 수 있다.

## 환경 요구사항

- Visual Studio 2022 (v145 toolset)
- Windows SDK 10.0+

## 빌드

```powershell
msbuild DummyDataGenerator-KHNam-CRA-3.vcxproj /p:Configuration=Debug /p:Platform=x64
```

빌드 결과물: `x64\Debug\DummyDataGenerator-KHNam-CRA-3.exe`

## 실행

```
x64\Debug\DummyDataGenerator-KHNam-CRA-3.exe
```

실행 위치와 `data/` 폴더가 같은 디렉토리에 있어야 한다.

## 사용법

### 입력 흐름

**1. 프로그램 시작 — 기존 데이터가 있는 경우**
```
기존 데이터 5건이 있습니다.
추가하시겠습니까? (1: 추가 / 2: 초기화):
```
- `1` 또는 그 외 입력: 기존 데이터 유지하고 뒤에 추가
- `2`: 기존 데이터 삭제 후 1번 ID부터 새로 시작

**2. 메인 메뉴**
```
=== 더미 데이터 생성기 ===
1. 더미 데이터 생성
0. 종료
선택:
```
- `1`: 더미 데이터 생성으로 진입
- `0`: 프로그램 종료
- 그 외 입력: "잘못된 입력입니다." 출력 후 메뉴 재표시

**3. 생성 수 입력**
```
생성할 수를 입력하세요:
```
- 양의 정수 입력 → 해당 수만큼 레코드 생성
- 비숫자 입력: "숫자를 입력해주세요." 출력 후 메뉴로 복귀

### 출력 결과

생성 완료 시:
```
N건 생성 완료. (총 M건)
```
- `N`: 이번에 생성한 수
- `M`: `data/records.json`에 저장된 전체 레코드 수

`data/records.json` 저장 형식 (기본 스키마 기준):
```json
[
  { "id": "1", "name": "name-1", "value": "value-1" },
  { "id": "2", "name": "name-2", "value": "value-2" }
]
```

## 스키마 커스터마이징

`data/default.json`에서 필드를 자유롭게 추가·제거할 수 있다.

```json
{
  "fields": [
    { "name": "id",    "type": "int",    "prefix": "" },
    { "name": "name",  "type": "string", "prefix": "name-" },
    { "name": "value", "type": "string", "prefix": "value-" }
  ]
}
```

- `prefix`가 빈 문자열이면 값은 순수 인덱스 숫자 (`1`, `2`, ...)
- `prefix`가 있으면 `prefix + 인덱스` 형태 (`name-1`, `name-2`, ...)
- `type: "int"` 필드를 ID로 인식해 자동 증가 채번에 사용

## 프로젝트 구조

```
src/            소스 파일
data/           데이터 파일 (default.json, records.json)
vendor/         서드파티 라이브러리 (nlohmann/json)
docs/           설계 문서
```
