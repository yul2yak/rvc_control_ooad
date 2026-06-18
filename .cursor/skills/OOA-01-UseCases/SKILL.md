---
name: ooa-01-usecases
description: >-
  docs/OOA/01-System-Requirements.md에서 Use Case Diagram, UC 텍스트, SSD를
  작성한다. docs/OOA/00-UseCase-Diagram.md 및 UseCases/UC-###.md 산출.
  OOA, use case diagram, SSD, system operation 작업 시 사용.
---
# OOA 1단계: Use Case Diagram · Use Case · SSD

`docs/OOA/01-System-Requirements.md`의 **FR·NFR**을 근거로 **Use Case Diagram → UC 텍스트 → SSD** 순으로 작성한다.

**입력:** `docs/OOA/01-System-Requirements.md` (필수, 없으면 중단)

**산출:**

| 순서 | 산출물 |
|------|--------|
| 1 | `docs/OOA/00-UseCase-Diagram.md` |
| 2 | `docs/OOA/UseCases/UC-###.md` |
| 3 | UC 파일 내 **SSD-UC-###-S##** (시나리오 1개 = SSD 1개) |

---

## 0. Use Case Diagram (선행)

- Actor: **Operator**(청소 **시작**만), **Environment**(장애물·먼지)
- UC와 FR 매핑, `<<include>>` / `<<extend>>` 관계
- Operator SSD: **세션 제어** (`startAutomaticCleaning` 등)만
- FR-002~ 자율 이동·청소: **include** — Operator 이벤트 **금지**
- Environment: `obstacleDetected`, `surroundedObstacleDetected`, `dustDetected`

---

## Use Case — 정의 · 목표 (OOAD)

| | 내용 |
|---|------|
| **정의** | Actor가 **:System** 를 사용해 목표를 달성하는 **텍스트 시나리오** |
| **목표** | FR을 Actor·include/extend 관점의 **검증 가능한 상호작용**으로 재구성 |

## SSD — 정의 · Actor 규칙

| | 내용 |
|---|------|
| **정의** | **Actor → :System** system event 순서 (블랙박스) |
| **Operator** | `startAutomaticCleaning()` 등 **시작/세션** 이벤트만 |
| **Environment** | 감지·자극 이벤트만 |
| **include (UC-002 등)** | Actor→System 화살표 **없음** — Note + system operation 표 |
| **System 내부** | SSD 표에 operation 열거; `S->>S` 상세는 **OOD SD** |

## 시나리오 ID · 파일 규칙

| 규칙 | 내용 |
|------|------|
| UC ID | UC-001=Start, UC-002=Forward(include), UC-003~005=extend (프로젝트 diagram 참고) |
| 시나리오 ID | `S01`(Typical), `S02`…(Alternative), `S9x`(Exceptional) |
| UC 문서 | `docs/OOA/UseCases/UC-###.md` |

---
## UC 문서 형식 (`UC-###.md`)

| 섹션 | 작성 규칙 |
|------|-----------|
| **Name** | `UC-###` + 짧은 영문 이름 + 목표 |
| **Actor** | Primary · include UC는 Actor 없음 명시 |
| **Pre-Requisites** | include/extend 전제 |
| **Typical / Alternative / Exceptional** | 단계마다 FR/NFR |
| **시나리오 ID 요약** | SSD 섹션명 |
| **Postconditions** | 성공 후 상태 |
| **Mermaid** | flowchart 분기 |

---
## SSD — UC 파일에 같이 작성

```markdown
# SSD-UC-##-S##
- **UC 시나리오:** UC-##-S##
- **Actor:** Operator | Environment | _(없음 — include)_
- **목적:**

(mermaid — Actor→System 이벤트만; include는 Note)

| System Event | System Operation | Parameters | FR/NFR |
```

---
## 체크리스트

- [ ] `00-UseCase-Diagram.md` — Actor, UC, include/extend, FR 매핑
- [ ] Operator SSD에 **moveForward** 등 자율 동작 **없음**
- [ ] UC-002 include — Actor→System 이벤트 없음
- [ ] Environment extend UC — 감지 이벤트만 Actor→System
- [ ] 시나리오 ID마다 SSD 1개 · FR/NFR 추적

## 완료 보고

`00-UseCase-Diagram` · UC 건수 · SSD(시나리오) 건수 · 경로 목록
