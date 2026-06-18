# System Test Scenarios

`System-Test/scenarios/ST-*.json` — **25건** (ST ≥ 20 요구 충족)

## 시나리오 목록

| ID | 이름 | trigger | ticks | FR | Capabilities (≥3) |
|----|------|---------|-------|-----|-------------------|
| ST-001 | Start cleaning forward north | start | 3 | FR-001, FR-002 | CAP-001, CAP-002, CAP-012 |
| ST-002 | Start cleaning forward east | start | 3 | FR-001, FR-002 | CAP-001, CAP-002, CAP-012 |
| ST-003 | Obstacle avoid turn right resume | obstacle | 1 | FR-003, UR-001 | CAP-003, CAP-004, CAP-005, CAP-007 |
| ST-004 | Obstacle avoid turn left resume | obstacle | 1 | FR-003, UR-001 | CAP-003, CAP-004, CAP-006, CAP-007 |
| ST-005 | Surrounded backward turn right | surrounded | 1 | FR-004, UR-001, UR-002 | CAP-003, CAP-008, CAP-005, CAP-007 |
| ST-006 | Surrounded backward turn left | surrounded | 1 | FR-004, UR-001, UR-002 | CAP-003, CAP-008, CAP-006, CAP-007 |
| ST-007 | Dust boost forward cleaning | dust | 1 | FR-005, UR-003 | CAP-009, CAP-010, CAP-002 |
| ST-008 | Multi-tick auto forward path | start | 5 | FR-002 | CAP-001, CAP-002, CAP-012 |
| ST-009 | Start cleaning forward south | start | 4 | FR-001, FR-002 | CAP-001, CAP-002, CAP-012 |
| ST-010 | Start cleaning forward west | start | 3 | FR-001, FR-002 | CAP-001, CAP-002, CAP-012 |
| ST-011 | Obstacle east heading turn right | obstacle | 1 | FR-003 | CAP-003, CAP-004, CAP-005, CAP-007 |
| ST-012 | Obstacle east heading turn left | obstacle | 1 | FR-003, UR-001 | CAP-003, CAP-004, CAP-006, CAP-007 |
| ST-013 | Auto forward then obstacle turn | start | 5 | FR-002, FR-003 | CAP-001, CAP-002, CAP-003, CAP-005, CAP-007 |
| ST-014 | Surrounded south heading recovery | surrounded | 1 | FR-004 | CAP-003, CAP-008, CAP-005, CAP-007 |
| ST-015 | Auto path with dust boost | start | 3 | FR-002, FR-005 | CAP-001, CAP-002, CAP-009, CAP-010 |
| ST-016 | Obstacle west heading turn right | obstacle | 1 | FR-003 | CAP-003, CAP-004, CAP-005, CAP-007 |
| ST-017 | Obstacle west heading turn left | obstacle | 1 | FR-003, UR-001 | CAP-003, CAP-004, CAP-006, CAP-007 |
| ST-018 | Surrounded west heading recovery | surrounded | 1 | FR-004 | CAP-003, CAP-008, CAP-005, CAP-007 |
| ST-019 | Start on dust cell forward | start | 1 | FR-001, FR-002 | CAP-001, CAP-002, CAP-012 |
| ST-020 | Long east corridor cleaning | start | 6 | FR-002 | CAP-001, CAP-002, CAP-012 |
| ST-021 | Obstacle south heading turn right | obstacle | 1 | FR-003 | CAP-003, CAP-004, CAP-005, CAP-007 |
| ST-022 | Auto obstacle avoid continue east | start | 6 | FR-002, FR-003 | CAP-001, CAP-002, CAP-003, CAP-005 |
| ST-023 | Surrounded east turn left | surrounded | 1 | FR-004, UR-001 | CAP-003, CAP-008, CAP-006, CAP-007 |
| ST-024 | Auto forward obstacle turn continue | start | 6 | FR-002, FR-003 | CAP-001, CAP-002, CAP-003, CAP-005, CAP-007 |
| ST-025 | Auto tick when L/R/F blocked must rotate | start | 5 | FR-004, UR-001 | CAP-003, CAP-008, CAP-006, CAP-007 |

## GUI 시뮬레이터 조작

| 키 | 동작 |
|----|------|
| S | startAutomaticCleaning (첫 tick = 세션 시작 + 1동작) |
| T | tick 1회 (전진·후진·회전 중 **하나**만) |
| R | 시나리오 ticks 실행 |
| [ / ] | manifest 시나리오 이전 / 다음 선택 |
| L | 선택한 ST 시나리오 로드 |
| F5 | custom_map.json 저장 |
| E | 편집 모드 토글 |
| O / D | 장애물 / 먼지 배치 |
| N | 기본 맵 리셋 |

편집 모드에서 그리드 클릭으로 obstacle·dust 배치. 경로는 tick마다 실시간 표시.

## 검증

`rvc_system_test`가 manifest의 JSON을 로드하고 `expectedPath`와 실제 `MapModel.path()`를 비교한다.
