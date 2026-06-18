# System Requirements (01)

## §0. 문서 개요

### 0.1 입력 (@)

| ID | 문서 | 경로 |
|----|------|------|
| IN-001 | Preliminary Requirements for RVC SW Controller | `docs/Preliminary-Requirements.md` |

### 0.2 시스템 정의 및 범위

| 항목 | 내용 |
|------|------|
| **시스템명** | RVC SW Controller (로봇 청소기 소프트웨어 제어) |
| **시스템 목적** | 가정용 표면의 **자동 청소·물걸레** 기능을 소프트웨어로 제어한다. |
| **포함(In Scope)** | 직진 청소 이동, 장애물 감지·회피(전진/후진·좌우 전환), 먼지 감지 시 청소 출력 강화 |
| **제외(Out of Scope)** | HW 제어의 상세 설계·구현; 자동 청소 기능 이외(모바일 앱, ML, 한 지점 순환 청소, 센서 추가·변경 자체) |

### 0.3 ID · 추적 규칙

- 모든 FR·NFR·보류·미해결 항목은 **IN-001** 출처 또는 하위 파생 근거를 갖는다.
- 출처 없는 요구 항목을 추가하지 않는다.
- **보류(Deferred)** 항목은 현재 FR로 승격하지 않는다.
- 수용기준은 원문에 명시된 내용과 **§4 해결 결정**을 포함한다.

### 0.4 청소 동작 invariant

- **청소·물걸레는 전진(forward) 이동 중에만** 수행한다 (FR-002).
- 후진·방향 전환(제자리 회전) 중에는 청소를 **중지**한다.
- 회피 maneuver 완료 후 **전진을 재개할 때** 청소를 **재개**한다 (FR-003, FR-004).

---

## §1. Functional Requirements (FR)

### FR-001 — 자동 청소 및 물걸레

| 필드 | 내용 |
|------|------|
| **명세** | 시스템은 가정용 표면(household surface)을 **자동으로** 청소하고 물걸레질(mop)한다. |
| **우선순위** | Must |
| **리스크** | — |
| **수용기준(원문)** | "An RVC automatically cleans and mops household surface." |
| **관련 NFR** | NFR-002 |
| **출처** | IN-001 · L2 |
| **확장성** | 한 지점 순환 청소(보류 DEF-002) 추가 시 본 FR 하위 시나리오 확장 가능 |

---

### FR-002 — 청소 중 직진 전진

| 필드 | 내용 |
|------|------|
| **명세** | 청소하는 동안 시스템은 **직진(forward)** 으로 이동한다. **전진 중에만** 청소·물걸레를 수행한다. |
| **우선순위** | Must |
| **리스크** | — |
| **수용기준(원문)** | "It goes straight forward while cleaning." |
| **수용기준(결정)** | 후진·회전 중 청소 중지; 전진 재개 시 청소 재개 (§0.4, UR-002) |
| **관련 NFR** | — |
| **출처** | IN-001 · L3 |
| **확장성** | — |

---

### FR-003 — 장애물 감지 시 회피 후 청소 재개

| 필드 | 내용 |
|------|------|
| **명세** | 센서가 장애물을 감지하면, 시스템은 청소를 **중지**하고, **우측 전환을 1차 시도**한다. 우측 전환이 불가하면 **좌측으로 전환**한 뒤, **청소를 재개하며 전진**한다. |
| **우선순위** | Must |
| **리스크** | — |
| **수용기준(원문)** | "If its sensors found an obstacle, it stops cleaning, turns aside left or right, and goes forward with cleaning." |
| **수용기준(결정)** | 우측 1차 → 불가 시 좌측 (UR-001); 전진 시 청소 재개 (§0.4) |
| **관련 NFR** | NFR-001, NFR-003 |
| **출처** | IN-001 · L4 |
| **확장성** | 센서 추가·변경(보류 DEF-001) 시 감지 소스만 교체 가능하도록 분리 |

---

### FR-004 — 전·좌·우 장애물 시 후진·회피

| 필드 | 내용 |
|------|------|
| **명세** | **전방·좌측·우측** 모두 장애물이 있으면, 시스템은 **청소를 중지**하고 **후진**한다. 이후 **우측 전환을 1차 시도**하고, 불가하면 **좌측으로 전환**한 뒤 **청소를 재개하며 전진**한다. |
| **우선순위** | Must |
| **리스크** | — |
| **수용기준(원문)** | "If there are obstacles in both front, left and right, it move backward and turn aside left or right, and goes forward." |
| **수용기준(결정)** | 후진·회전 중 청소 중지; 우측 1차 → 불가 시 좌측 (UR-001); 전진 재개 시 청소 재개 (UR-002, §0.4) |
| **관련 NFR** | NFR-001, NFR-003 |
| **출처** | IN-001 · L5 |
| **확장성** | 센서 추가·변경(보류 DEF-001) 시 방향별 감지 소스 확장 가능 |

---

### FR-005 — 먼지 감지 시 청소 출력 강화

| 필드 | 내용 |
|------|------|
| **명세** | 먼지를 감지하면, 시스템은 **3초 동안** 청소 출력을 **높인다(power up)**. |
| **우선순위** | Must |
| **리스크** | — |
| **수용기준(원문)** | "If it detects dust, power up the cleaning for a while." |
| **수용기준(결정)** | 강화 지속 시간 **3초** (UR-003); 추후 튜닝 가능 |
| **관련 NFR** | NFR-003, NFR-004 |
| **출처** | IN-001 · L6 |
| **확장성** | 센서 추가·변경(보류 DEF-001) 시 먼지 감지 소스 교체 가능; **강화 시간(3초)은 설정·튜닝 대상** |

---

## §2. Non-Functional Requirements (NFR)

### NFR-001 — HW 제어 상세 범위 제외

| 필드 | 내용 |
|------|------|
| **범주** | Scope / Architecture |
| **요구** | HW 제어에 대한 **상세 설계·구현**은 본 시스템 범위에 **포함하지 않는다**. |
| **근거** | SW Controller 관점; HW는 추상화 대상 |
| **검증 방법** | 설계·구현 산출물에 HW 레지스터·드라이버 상세 명세 부재 확인 |
| **관련 FR** | FR-003, FR-004, FR-005 |
| **출처** | IN-001 · L7 |

---

### NFR-002 — 자동 청소 기능 집중

| 필드 | 내용 |
|------|------|
| **범주** | Scope |
| **요구** | 본 시스템은 **자동 청소 기능(automatic cleaning function)** 에만 초점을 둔다. |
| **근거** | 명시적 범위 한정 |
| **검증 방법** | 요구·설계·구현이 FR-001~005 및 관련 NFR 외 기능을 포함하지 않음 |
| **관련 FR** | FR-001 |
| **출처** | IN-001 · L8 |

---

### NFR-003 — 센서 추상화 (확장 대비)

| 필드 | 내용 |
|------|------|
| **범주** | Extensibility / Maintainability |
| **요구** | 장애물·먼지 **감지**는 구체 센서 HW가 아닌 **추상화된 감지 capability** 로 표현 가능해야 한다. (향후 DEF-001 대비) |
| **근거** | "Future: The RVC will add or change sensors." — 현재 FR은 감지 **행위**만 요구 |
| **검증 방법** | OOA/OOD에서 센서 타입명·HW API 직접 노출 없이 black-box 감지 이벤트로 기술 |
| **관련 FR** | FR-003, FR-004, FR-005 |
| **출처** | IN-001 · L4–L6, L11 (보류 DEF-001 연계) |

---

### NFR-004 — 먼지 강화 시간 튜닝 가능

| 필드 | 내용 |
|------|------|
| **범주** | Configurability / Maintainability |
| **요구** | 먼지 감지 시 청소 출력 강화 **지속 시간(기본 3초)** 은 구현에서 **설정·튜닝** 가능해야 한다. |
| **근거** | UR-003 결정 — 초기값 3초, 추후 환경·성능에 따라 조정 필요 |
| **검증 방법** | 설정값 변경 시 FR-005 강화 지속 시간이 그에 따라 변경됨을 테스트로 확인 |
| **관련 FR** | FR-005 |
| **출처** | UR-003 (stakeholder 결정) |

---

## §3. 보류 (Deferred)

원문 *Future or Extended Requirements* — **현재 FR 승격 금지**.

| ID | 내용 (원문) | 출처 | 연관 FR/NFR |
|----|-------------|------|-------------|
| DEF-001 | The RVC will add or change sensors. | IN-001 · L11 | NFR-003, FR-003~005 확장성 |
| DEF-002 | It will be able to circulate one spot for a while. | IN-001 · L12 | FR-001 확장성 |
| DEF-003 | It will have to communicate with a mobile app. | IN-001 · L13 | NFR-002 (범위 외) |
| DEF-004 | It can do machine learning and inferring for more efficient cleaning. | IN-001 · L14 | NFR-002 (범위 외) |

---

## §4. 해결 결정 (Resolved)

| ID | 항목 | 결정 | 관련 FR | 근거 |
|----|------|------|---------|------|
| UR-001 | 좌/우 전환 선택 | **우측 1차** 시도; 우측 전환 불가 시 **좌측** 전환 | FR-003, FR-004 | stakeholder 결정 |
| UR-002 | FR-004 후 청소 상태 | 후진·회전 중 청소 **중지**; **전진 재개 시 청소 재개** (FR-002·§0.4와 동일 원칙) | FR-002, FR-004 | stakeholder 결정; "전진할 때만 청소" |
| UR-003 | 먼지 강화 지속 시간 | **3초**; 추후 상황에 따라 **튜닝** (NFR-004) | FR-005 | stakeholder 결정 |

**미해결 항목:** 없음

---

## §5. 추적성 매트릭스 (원문 → ID)

| 출처 (IN-001) | ID | 비고 |
|---------------|-----|------|
| L2 — automatically cleans and mops household surface | FR-001 | |
| L3 — goes straight forward while cleaning | FR-002 | |
| L4 — obstacle: stop, turn left/right, forward with cleaning | FR-003 | UR-001 해결 |
| L5 — front+left+right obstacles: backward, turn, forward | FR-004 | UR-001, UR-002 해결 |
| L6 — detects dust, power up cleaning for a while | FR-005 | UR-003 → 3초 |
| L7 — do not consider HW controls detail design/implementation | NFR-001 | |
| L8 — only focus on automatic cleaning function | NFR-002 | |
| L11 — add or change sensors | DEF-001 | → NFR-003 |
| L12 — circulate one spot | DEF-002 | |
| L13 — communicate with mobile app | DEF-003 | |
| L14 — machine learning for efficient cleaning | DEF-004 | |

**커버리지:** IN-001 현재 범위(L2–L8) 전 항목 매핑 완료. Future(L11–L14) → 보류 4건.

---

## §6. 요약

| 구분 | 건수 |
|------|------|
| FR | 5 |
| NFR | 4 |
| 보류 (Deferred) | 4 |
| 해결 결정 (Resolved) | 3 |
| 미해결 (Unresolved) | 0 |
