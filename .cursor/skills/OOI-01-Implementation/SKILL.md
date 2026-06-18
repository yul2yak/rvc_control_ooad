---
name: ooi-01-implementation
description: >-
  DCD·SD·Traceability-Matrix와 FR/NFR을 근거로 C++ 구현, gtest, GUI
  시뮬레이터 System test를 수행한다. OOI, 구현, unit test, system test,
  simulator 작업 시 사용.
---
# OOI: 구현 · Unit/Integration 테스트 · GUI 시뮬레이터(System test)

**DCD·SD·traceability**와 **FR/NFR**(입력 문서)를 근거로 구현·테스트한다. OOD의 **클래스명·public API·협력**과 일치. 임의 추가 금지.

**입력(필수, `@`):**

- `docs/OOA/01-System-Requirements.md`
- `docs/OOD/DCD/DCD.md`
- `docs/OOD/SD/`
- `docs/OOD/Traceability-Matrix.md`
- `docs/OOA/UseCases/` (UC·SSD)

없으면 `ooa-*` / `ood-*` 완료 후 중단.

**산출:** 프로젝트 **기존** 디렉터리 규약 우선 · 없을 때 예:

| 레벨 | 경로 (예) |
|------|-----------|
| 구현 | `include/`, `src/` |
| Unit/Integration | `test/` |
| GUI 시뮬레이터 | `sim/` |
| System test | `docs/System-Test/Capability-Catalog.md`, `Scenarios.md`, `System-Test/` |
| 빌드 | `CMakeLists.txt` — lib, tests, simulator 타깃 |

**금지:** OOA/OOD 문서 수정 · 01 문서 **보류(DEF 등)** 구현 · 설계·NFR 범위 밖 API

---
## OOI 목표

| # | 목표 |
|---|------|
| 1 | traceability **Class.Method** 코드 심볼 |
| 2 | SD 메시지 순서·분기 구현 |
| 3 | gtest로 **입력 FR/NFR** 검증 (branch·line **90%+**, Project Rule) |
| 4 | GUI 시뮬레이터로 System test 수행 가능 |
| 5 | **ST ≥ 20**, **ST 1건당 기능이 3개 이상 수행되는** (복잡한 시나리오) |

---
## Rules (프로젝트 `.cursor/rules/` 확인)

| Rule | 적용 |
|------|------|
| `cpp-impl.mdc` | C++14, SOLID, 헤더/`.cpp` |
| `testing.mdc` | Unit/Integration → WSL·gtest·ctest · System → GUI 시뮬레이터 |
| `gtest-framework.mdc` | FR/NFR 주석, 커버리지 90%+ |

**Agent:** Unit/Integration은 **WSL**에서 빌드·`ctest`. System test는 **시뮬레이터/시나리오 산출**까지; unit test, integration test, system test가 모두 통과할 때까지 테스트 결과를 피드백 받으며 구현한다. System test는 GUI(또는 `system_test` 러너가 시뮬레이터 기동).

---
## 1. src (DCD 1:1)

- DCD **설계 클래스**마다 헤더 + `.cpp` (프로젝트 네임스페이스·경로 따름)
- **operation 이름** = traceability **Method** · **시그니처** = DCD
- SD **메시지** = 메서드 호출 순서

---
## 2. Unit / Integration

- gtest/gmock · 주석 `FR-###` / `NFR-###` · UC/SD ID 권장
- **01 문서의 모든 현재 FR/NFR**에 대응 테스트(개수는 프로젝트마다 다름)
- WSL: cmake · ctest · 커버리지(가능 시, 추정 금지)

---
## 3. GUI 시뮬레이터

- 시뮬레이터는 GUI 환경의 system test를 진행한다 (`sim/`).
- 시뮬레이터에는 RVC의 위치와 시작 방향, 장애물과 먼지의 위치가 시각적으로 보인다.
- Test가 진행될 때 RVC가 이동한 경로가 표시되며, 매 tick마다 RVC state와 움직임을 모니터링할 수 있어야 한다.
- System test 입력은 JSON map 형태: map 크기, RVC 위치·시작 방향, obstacle·dust 위치. 실행 시 매 tick마다 RVC가 자동 이동하며 기대 움직임과 일치하는지 확인한다.
- 사용자가 map 크기, RVC 위치·초기 방향, obstacle·dust 위치를 수정할 수 있어야 한다. 특정 tick만큼 test를 진행하고, 환경을 JSON으로 저장할 수 있어야 한다. 개발자가 기대 경로를 작성해 동일 동작을 검증할 수 있어야 한다.

## 체크리스트

- [ ] DCD·traceability 코드 일치
- [ ] WSL ctest, FR/NFR 주석, 커버리지 90%+ (또는 미측정 사유)
- [ ] 시뮬레이터 빌드 (`sim/`)
- [ ] ST≥20, ST당 기능≥3, capability-catalog, 단일기능 ST 없음
- [ ] Project Rules 준수, 보류·범위 밖 구현 없음

## 완료 보고

ctest · 커버리지 · 시뮬레이터 경로/실행 · ST 개수 · ST당 기능 수 · traceability 누락
