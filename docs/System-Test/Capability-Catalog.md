# System Test — Capability Catalog

RVC SW Controller GUI 시뮬레이터(`sim/`) 및 JSON 시나리오(`System-Test/scenarios/`) 기반 System test capability 목록.

## Capability 목록

| ID | Capability | 관련 FR/NFR | DCD Class.Method |
|----|------------|-------------|------------------|
| CAP-001 | 자동 청소·물걸레 시작 | FR-001, NFR-002 | RvcController.startAutomaticCleaning, CleaningService.activateCleaningAndMopping |
| CAP-002 | 전진 중 청소·물걸레 동기화 | FR-002, §0.4 | MovementController.moveForwardWithCleaning, CleaningService.syncForwardCleaningState |
| CAP-003 | 장애물 감지 시 청소 중지 | FR-003, §0.4 | CleaningService.stopCleaning |
| CAP-004 | 우측 전환 가능 판단 | FR-003, UR-001, NFR-003 | MovementController.canTurnRight, IObstacleDetector.isRightTurnFeasible |
| CAP-005 | 우측 회피 전환 | FR-003, UR-001 | MovementController.turnRight |
| CAP-006 | 좌측 회피 전환 (fallback) | FR-003, FR-004, UR-001 | MovementController.turnLeft |
| CAP-007 | 회피 후 전진·청소 재개 | FR-003, FR-004, UR-002 | MovementController.resumeForwardWithCleaning |
| CAP-008 | 삼방향 장애물 후진 | FR-004, §0.4 | MovementController.moveBackward |
| CAP-009 | 먼지 감지 | FR-005, NFR-003 | IDustDetector.isDustPresent |
| CAP-010 | 청소 출력 강화 (3초) | FR-005, UR-003, NFR-004 | CleaningService.boostCleaningPower |
| CAP-011 | 청소 출력 기본 복원 | FR-005, NFR-004 | CleaningService.restoreDefaultCleaningPower |
| CAP-012 | 경로 시각화·tick 모니터링 | NFR-002 | SimulationEngine, MapModel.path |

## ST당 최소 capability 조합 (≥3)

각 System test 시나리오는 **3개 이상** capability를 조합한다. 단일 capability만 검증하는 ST는 없다.

| ST | Capabilities (≥3) |
|----|-------------------|
| ST-001 | CAP-001, CAP-002, CAP-012 |
| ST-003 | CAP-003, CAP-004, CAP-005, CAP-007 |
| ST-005 | CAP-003, CAP-008, CAP-005, CAP-007 |
| ST-007 | CAP-009, CAP-010, CAP-002 |
| ST-013 | CAP-001, CAP-002, CAP-003, CAP-005, CAP-007 |
| ST-015 | CAP-001, CAP-002, CAP-009, CAP-010 |
| ST-024 | CAP-001, CAP-002, CAP-003, CAP-005, CAP-007 |

(전체 24건 — `Scenarios.md` 참고)

## 실행

| 대상 | 명령 (프로젝트 루트) |
|------|----------------------|
| GUI 시뮬레이터 | `./build/rvc_sim` (SFML 필요) |
| System test 러너 | `./build/rvc_system_test` |
| Unit/Integration | `ctest --test-dir build` (WSL) |

## JSON map 필드

| 필드 | 설명 |
|------|------|
| width, height | Grid 크기 |
| rvc | 시작 위치·heading (`N`/`E`/`S`/`W`) |
| obstacles, dust | 좌표 배열 |
| ticks | 실행 tick 수 |
| trigger | `start` \| `obstacle` \| `surrounded` \| `dust` |
| expectedPath | 기대 RVC 경로 (검증용) |
