# ISO/IEC 25023:2016 — Response Measure Reference

QAS **Response Measure** 선택용 요약. 정본: **ISO/IEC 25023:2016** (품질 모델: **ISO/IEC 25059:2023**).

## Measure ID 형식

`{SubcharCode}-{serial}-{G|S}`

- **SubcharCode**: characteristic + subcharacteristic 약어 (예: `PTb` = Performance efficiency · Time behaviour)
- **G**: Generic (범용) · **S**: Specific (특정 상황)

표준은 **목표값·등급을 정하지 않음** — QAS 작성 시 프로젝트 맥락에서 Target value를 정의한다.

---

## ISO/IEC 25010 Characteristics (8)

| Code | Characteristic |
|------|----------------|
| F | Functional suitability |
| P | Performance efficiency |
| C | Compatibility |
| U | Usability |
| R | Reliability |
| S | Security |
| M | Maintainability |
| Po | Portability |

---

## RVC SW Controller 프로젝트 우선 참조 Measure

요구(01-System-Requirements)와 직접 연관 가능한 measure 위주.

### Functional suitability

| ID | Name | Function (요약) | QAS 활용 |
|----|------|---------------|----------|
| **FCp-1-G** | Functional coverage | X = 1 − A/B (A=누락 기능, B=명세 기능) | FR-001~005 커버리지 |
| **FCr-1-G** | Functional correctness | X = 1 − A/B (A=오류 기능, B=평가 기능) | 장애물 회피·청소 재개 정확성 |
| **FAp-1-G** | Functional appropriateness | X = 1 − A/B (특정 사용목표 대비 부적절 기능) | 청소 시나리오 적합성 |

### Performance efficiency — Time behaviour

| ID | Name | Function (요약) | QAS 활용 |
|----|------|---------------|----------|
| **PTb-1-G** | Mean response time | X = ΣAi/n (Ai=요청~응답 시간) | 감지→동작 지연 |
| **PTb-2-G** | Response time adequacy | X = A/B (A=측정 평균 응답, B=요구 응답시간) | tick·ms 단위 임계 |
| **PTb-4-G** | Mean turnaround time | X = Σ(Bi−Ai)/n (작업 완료 시간) | 먼지 강화 **3초** 지속 (FR-005, UR-003) |

### Reliability — Fault tolerance

| ID | Name | Function (요약) | QAS 활용 |
|----|------|---------------|----------|
| **RFt-1-G** | Fault avoidance | X = A/B (통제된 fault 패턴/전체) | 장애물·포위 상황 회피 |
| **RFt-4-G** | Mean fault notification time | X = Σ(Ai−Bi)/n (감지~통지) | 센서 이벤트→제어 반응 |

### Maintainability — Modularity

| ID | Name | Function (요약) | QAS 활용 |
|----|------|---------------|----------|
| **MMo-1-G** | Coupling of components | X = A/B (독립 구현 컴포넌트/요구 독립 컴포넌트) | NFR-003 센서 추상화 |
| **MMo-2-G** | Acceptable cyclomatic complexity | X = 1 − A/B (임계 초과 모듈/전체 모듈) | 모듈 결합도·복잡도 |

### Maintainability — Reusability

| ID | Name | Function (요약) | QAS 활용 |
|----|------|---------------|----------|
| **MRe-1-G** | Reusability of assets | X = A/B (재사용 설계 자산/요구 재사용 자산) | 감지 capability 재사용 |
| **MRe-2-G** | Coding rules conformity | X = A/B (규칙 준수 모듈/전체) | 코딩 규약 준수 |

### Maintainability — Analysability

| ID | Name | Function (요약) | QAS 활용 |
|----|------|---------------|----------|
| **MAn-1-G** | Product log completeness | X = A/B (로그 기록 연산/감사 필요 연산) | 시뮬·테스트 추적 |
| **MAn-3-G** | Diagnosis function sufficiency | X = A/B (구현 진단 기능/요구 진단 기능) | 결함 원인 분석 |

### Maintainability — Modifiability

| ID | Name | Function (요약) | QAS 활용 |
|----|------|---------------|----------|
| **MMd-1-G** | Modification efficiency | ΣΣ(Aij/Bij)/m (실제/예상 수정 시간) | 센서 교체·설정 변경 효율 |
| **MMd-2-G** | Modification capability | X = A/B (기한 내 완료 수정/요구 수정) | NFR-004 튜닝 가능성 |

### Maintainability — Testability

| ID | Name | Function (요약) | QAS 활용 |
|----|------|---------------|----------|
| **MTe-1-G** | Testability of functions | X = A/B (테스트 가능 기능/전체 기능) | gtest·System test 커버 |
| **MTe-3-G** | Autonomous testability | X = A/B (stub 가능 테스트/외부 의존 테스트) | 시뮬레이터 독립 테스트 |

### Compatibility — Co-existence

| ID | Name | Function (요약) | QAS 활용 |
|----|------|---------------|----------|
| **CCo-1-G** | Co-existence of components | X = A/B (공존 성공 컴포넌트/평가 컴포넌트) | HW 추상화·다중 센서 확장 |

### Portability — Adaptability (Clause 8.9.1)

25023:2016 §8.9.1에 정의된 Generic measure (접두사·ID는 **표준 표**에서 확인):

| Name (2016) | Function (요약) | QAS 활용 |
|-------------|---------------|----------|
| Hardware environmental adaptability | X = 1 − A/B | HW·센서 환경 변경 (DEF-001 대비) |
| System software environmental adaptability | X = 1 − A/B | 플랫폼·빌드 환경 이식 |
| Operational environment adaptability | X = 1 − A/B | 시뮬·실기 운영 환경 전환 |

> 센서 추상화(NFR-003) QAS에는 Portability보다 **MMo-1-G**, **MRe-1-G**가 더 직접적인 경우가 많다.

---

## 전체 Subcharacteristic · Measure ID 접두사

| Subchar | ID prefix | Subcharacteristic |
|---------|-----------|-------------------|
| Functional completeness | FCp | |
| Functional correctness | FCr | |
| Functional appropriateness | FAp | |
| Time behaviour | PTb | |
| Resource utilization | PRu | |
| Capacity | PCa | |
| Co-existence | CCo | |
| Interoperability | CIn | |
| Appropriateness recognizability | IAr | |
| Learnability | ILe | |
| Operability | IOp | |
| User error protection | IEp | |
| User interface aesthetics | IUa | |
| Accessibility | ISd | |
| Maturity | RMa | |
| Availability | RAv | |
| Fault tolerance | RFt | |
| Recoverability | RRe | |
| Confidentiality | SCo | |
| Integrity | SIn | |
| Non-repudiation | SNr | |
| Accountability | SAc | |
| Authenticity | SAt | |
| Modularity | MMo | |
| Reusability | MRe | |
| Analysability | MAn | |
| Modifiability | MMd | |
| Testability | MTe | |
| Adaptability | (§8.9.1 표 참조) | Portability |
| Installability | (§8.9.2 표 참조) | Portability |
| Replaceability | (§8.9.3 표 참조) | Portability |

상세 정의·추가 measure는 ISO/IEC 25023:2016 Clause 8(또는 개정판 Clause 7) 표를 참조한다.

---

## Target value 설정 가이드 (Annex A 요지)

ISO 25023은 목표값을 규정하지 않는다. QAS에서 다음 중 하나로 정당화한다.

| 방식 | 설명 |
|------|------|
| **Conformance** | 01-System-Requirements 수용기준·UR (예: 3초, canTurnRight) |
| **Benchmark** | 기존 제품·이전 버전 대비 |
| **Time series** | 릴리스·부하에 따른 추세 |
| **Assumption** | 근거 부족 시 가정 명시 + stakeholder 확인 |

내부 measure(정적 리뷰·구조)와 외부 measure(실행·테스트)를 Verification에 구분한다.

---

## 사용자 정의 Measure (표준 외)

ISO 25023 §2e: 표준에 없으면 **ISO/IEC 25021** QME 형식으로 정의.

```markdown
- **Custom measure ID:** CM-### 
- **ISO 25010 mapping:** [characteristic · subcharacteristic]
- **Name:** 
- **QMEs:** A = …, B = …
- **Function:** X = …
- **Rationale:** 왜 표준 measure로 대체 불가한지
```
