# OOD 1단계: Sequence Diagram + Design Class Diagram
**SSD system operation**을 근거로 **SD(내부 협력)** 와 **DCD(설계 클래스)** 를 **함께·반복적으로** 정의한다.
**입력(필수):** `docs/OOA/UseCases/UC-*.md` · `docs/OOA/03-Domain-Model.md`
**산출:**
- `docs/OOD/SD/SD-UC-###-S##.md` — SSD 시나리오 1개 = SD 1개
- `docs/OOD/DCD/DCD.md` — SD를 그릴 때마다 **누적·갱신**하는 통합 Class Diagram
- `docs/OOD/Traceability-Matrix.md` — **UC SSD SD method(DCD operation)** 통합 추적 표 (작업 **마지막**에 갱신)
**금지:** SSD·Domain Model 수정, **메서드 구현체(.cpp)**
---
| | SD (dynamic) | DCD (static) |
|---|--------------|--------------|
| **표현** | 객체 **message** 시간 순 | **class** · **operation 시그니처** · attribute |
| **관계** | SD에서 메시지를 그리면 → 수신 객체에 **operation** 후보가 생김 | DCD에 반영 후 다음 SD에서 lifeline으로 사용 |
| **방식** | 시나리오마다 SD 작성 | **같은 작업 세션**에서 DCD.md 갱신 (wall of classes + scenarios) |
---
## SSD vs SD
| SSD (OOA) | SD (OOD) |
|-----------|----------|
| Actor → `:System` event | system operation → **내부 객체** message |
| 블랙박스 | `:System` **분해** |
## DCD — 정의 · 목표
| | 내용 |
|---|------|
| **정의** | 소프트웨어 **설계 클래스**·interface·operation **시그니처**·attribute (구현 없음) |
| **목표** | SD lifeline을 **타입**으로 고정 · 컴파일 가능한 API 초안 |
| **Domain Model과 차이** | 개념 클래스 → 설계 클래스로 **매핑 표** 필수; 표기는 UML classDiagram + `+operation()` |
| **SOLID** | DCD·interface 분해 시 **SRP·OCP·LSP·ISP·DIP** 준수; 클래스 추가/병합 시 위반 여부를 §변경 이력에 1줄 기록 |
## lifeline · 클래스 규칙
- Domain Model 개념 → 설계 클래스(이름 유사 권장, representation gap 축소).
- Actor는 SD에만 외부 lifeline; DCD에는 보통 미포함.
---
## 작성 절차 (시나리오 1건당)
| # | SD | DCD |
|---|-----|-----|
| 1 | UC에서 SSD·System Operation 추출 | — |
| 2 | Domain Model 개념 매핑 | 동일 매핑을 `DCD.md` §매핑에 유지 |
| 3 | GRASP로 책임·lifeline 결정 | 클래스 등장/갱신 |
| 4 | `sequenceDiagram` 작성 | SD에서 나온 **message → operation** DCD에 추가 |
| 5 | SSD SD 매핑 표 | 신규/변경 class·operation을 DCD §변경 이력에 기록 |
| 6 | — | Mermaid `classDiagram` 전체 갱신 |
| 7 | — | 모든 SD·DCD 반영 후 **`Traceability-Matrix.md`** 작성·갱신 (누락 행 없음) |
---
## SD 파일 (`SD-UC-###-S##.md`)
```markdown
# SD-UC-###-S##
- **UC / SSD:** UC-###-S## / SSD-UC-###-S##
- **System Operation(주):** handleXxx()
## Lifelines → DCD 클래스
| Lifeline | DCD 클래스 | Domain 개념 |
## Sequence Diagram
(mermaid)
## SSD SD 매핑
| SSD Operation | SD message | To |
## DCD 갱신 (이 시나리오)
| 클래스 | 추가/확정 operation |
```