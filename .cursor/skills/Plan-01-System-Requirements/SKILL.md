---
name: plan-01-system-requirements
description: >-
  Preliminary Requirements 등 @ 입력 문서에서 FR·NFR(ISO/IEC 25010 품질속성
  포함) System Requirements를 추출한다. docs/OOA/01-System-Requirements.md
  산출(§0~§6). UP Inception, 요구사항 분석, FR-###, NFR-###, 품질속성 작업 시
  사용.
---
# Planning (UP Inception) 단계: System Requirements 분석 (FR · NFR)

`@` 요구 문서만 근거로 FR·NFR 분석 (OOAD · UP Inception). 임의 추가 금지.

**입력:** `@` 문서 1개 이상 (예: `docs/Preliminary-Requirements.md`). 없으면 중단.

**산출물:** `docs/OOA/01-System-Requirements.md` (**§0~§6** — §7 이후는 **OOA-01** 담당)

**후속:** **OOA-01** — 동일 파일에 §7 QA · §8 QAS · NFR 역참조 갱신

---

## 분석 항목

| # | 할 일 | 이유 |
|---|--------|------|
| 1 | **system 정의 및 범위** — 대상 시스템 범위(포함/제외) | 이후 분석 기준 |
| 2 | **FR** — 기능·행위 추출, black-box·의도 수준 서술, 1건=검증 단위 | 분석은 무엇을 하는지; 내부 설계·UI/디바이스명 최소 |
| 3 | **NFR** — 품질·제약·성능 등(FR과 분리, **ISO 25010 품질속성** 매핑) | Supplementary Specification |
| 4 | **FR 품질 암시 → NFR** — 수용기준·UR·§invariant에서 시간·정확성·확장 등을 NFR로 승격·보강 | 품질요구가 NFR에 명시적으로 존재 |
| 5 | **추적성** — 원문→`FR-###`/`NFR-###`/보류/미해결, 누락 없음 | 출처 없는 항목 금지 |
| 6 | **보류** — 향후·범위 외(원문만), 현재 FR 승격 금지 | |
| 7 | **확장성** — 변경 영향 FR/NFR 표시, ID 고정 | 하위 OOA 연계 |

---

## 필드 정의

**FR 필드:** `FR-###` · 제목 · 명세 · 우선순위 · 리스크 · 수용기준(원문) · 수용기준(결정, 해당 시) · 관련 NFR · 출처 · 확장성(선택)

**NFR 필드:**

| 필드 | 내용 |
|------|------|
| `NFR-###` | ID |
| 제목 | 짧은 이름 |
| **범주** | Scope / Architecture / **Quality** 등 |
| **ISO 25010 characteristic** | 품질 NFR 필수; Scope·Architecture NFR은 `—` |
| **ISO 25010 subcharacteristic** | 품질 NFR 필수; Scope·Architecture NFR은 `—` |
| 요구 | 검증 가능한 수준으로 서술 |
| 근거 | 원문·UR·FR 연계 |
| 검증 방법 | 리뷰·테스트 등 |
| 관련 FR | |
| **관련 QA** | OOA-01 전 `—` (OOA-01이 `QA-###` 역기입) |
| **관련 QAS** | OOA-01 전 `—` (OOA-01이 `QAS-###` 역기입) |
| 출처 | |

### FR → NFR 품질 승격 힌트

| FR 수용기준·UR | NFR 후보 (ISO 25010) |
|----------------|----------------------|
| 전진 중에만 청소 (§0.4, UR-002) | Functional suitability · Functional correctness |
| 장애물 회피·`canTurnRight` (UR-001) | Functional correctness · Reliability · Fault tolerance |
| 먼지 강화 **3초** (UR-003) | Performance efficiency · Time behaviour |
| 센서·감지 (NFR-003과 연계) | Maintainability · Modularity |

원문·FR에 이미 NFR로 있는 항목은 **중복 ID 금지** — 기존 NFR 보강만.

---

## 산출물 구조 (`01-System-Requirements.md` §0~§6)

```markdown
# System Requirements (01)

## §0. 문서 개요
### 0.1 입력 (@)
### 0.2 시스템 정의 및 범위
### 0.3 ID · 추적 규칙
### 0.4 (도메인 invariant, 해당 시)

## §1. Functional Requirements (FR)
### FR-### — [제목]
(표: FR 필드)

## §2. Non-Functional Requirements (NFR)
### NFR-### — [제목]
(표: NFR 필드 — ISO 25010 · 관련 QA/QAS 포함)

## §3. 보류 (Deferred)

## §4. 해결 결정 (Resolved)

## §5. 추적성 매트릭스 (원문 → ID)

## §6. 요약
```

**Plan-01에서 작성하지 않음:** §7 QA · §8 QAS · QAS 다이어그램 · ISO 25023 Response Measure → **OOA-01**

---

## 체크리스트

- [ ] §0에 `@` 문서, FR/NFR/보류/미해결·추적·출처 규칙 충족
- [ ] 품질 NFR(범주≠Scope/Architecture)에 **ISO 25010 characteristic · subcharacteristic** 기재
- [ ] FR 수용기준·UR의 품질 암시가 NFR에 반영(신규 또는 기존 NFR 보강)
- [ ] NFR `관련 QA`·`관련 QAS`는 `—` 로 두고 §7·§8 비워 둠(OOA-01 대기)
- [ ] 산출 경로: `docs/OOA/01-System-Requirements.md` (§0~§6)

## 완료 보고

입력 문서 · 경로 · FR/NFR/보류/미해결 건수 · **ISO 25010 매핑 NFR 건수** · 미해결 요약
