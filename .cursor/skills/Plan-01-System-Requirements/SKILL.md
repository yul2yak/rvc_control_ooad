---
name: plan-01-system-requirements
description: >-
  Preliminary Requirements 등 @ 입력 문서에서 FR·NFR System Requirements를
  추출한다. docs/OOA/01-System-Requirements.md 산출. UP Inception, 요구사항
  분석, FR-###, NFR-### 작업 시 사용.
---
# Planning (UP Inception) 단계: System Requirements 분석 (FR · NFR)

`@` 요구 문서만 근거로 FR·NFR 분석 (OOAD · UP Inception). 임의 추가 금지.

**입력:** `@` 문서 1개 이상 (예: `docs/Preliminary-Requirements.md`). 없으면 중단.

**산출물:** `docs/OOA/01-System-Requirements.md`

## 분석 항목

| # | 할 일 | 이유 |
|---|--------|------|
| 1 | **system 정의 및 범위** — 대상 시스템 범위(포함/제외) | 이후 분석 기준 |
| 2 | **FR** — 기능·행위 추출, black-box·의도 수준 서술, 1건=검증 단위 | 분석은 무엇을 하는지; 내부 설계·UI/디바이스명 최소 |
| 3 | **NFR** — 품질·제약·성능 등(FR과 분리, ID 상호참조) | Supplementary Specification |
| 4 | **추적성**— 원문→`FR-###`/`NFR-###`/보류/미해결, 누락 없음 | 출처 없는 항목 금지 |
| 5 | **보류**— 향후·범위 외(원문만), 현재 FR 승격 금지 | |
| 6 | **확장성**— 변경 영향 FR/NFR 표시, ID 고정 | 하위 OOA 연계 |

**FR 필드:** `FR-###` · 제목 · 명세 · 우선순위·리스크·수용기준(원문만) · 관련 NFR · 출처 · 확장성(선택)

**NFR 필드:** `NFR-###` · 범주 · 요구(가능하면 검증 가능) · 근거 · 검증 방법 · 관련 FR · 출처

## 체크리스트

- [ ] §0에 `@` 문서, FR/NFR/보류/미해결·추적·출처 규칙 충족, 범위 밖 산출물 없음
- [ ] 산출 경로: `docs/OOA/01-System-Requirements.md`

## 완료 보고

입력 문서 · 경로 · FR/NFR/보류/미해결 건수 · 미해결 요약
