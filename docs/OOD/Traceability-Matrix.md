# Traceability Matrix (OOD)

## 1. 입력

| 문서 | 경로 |
|------|------|
| System Requirements | `docs/OOA/01-System-Requirements.md` |
| Use Case · SSD | `docs/OOA/UseCases/` |
| Domain Model | `docs/OOA/02-Domain-Model.md` |
| SD | `docs/OOD/SD/` |
| DCD | `docs/OOD/DCD/DCD.md` |

## 2. 통합 추적 표

| UC | Scenario | FR/NFR | SSD System Operation | SD 파일 | SD message (순서) | DCD Class | DCD Method | Domain 개념 | Code Symbol |
|----|----------|--------|----------------------|---------|-------------------|-----------|------------|-------------|-------------|
| UC-001 | UC-001-S01 | FR-001, NFR-002 | startAutomaticCleaning | SD-UC-001-S01.md | 1. startAutomaticCleaning() | RvcController | +startAutomaticCleaning(): void | RVC | RvcController.startAutomaticCleaning |
| UC-001 | UC-001-S01 | FR-001 | startAutomaticCleaning | SD-UC-001-S01.md | 2. activateCleaningAndMopping() | CleaningService | +activateCleaningAndMopping(): void | CleaningOutput, RVC | CleaningService.activateCleaningAndMopping |
| UC-001 | UC-001-S01 | FR-002, §0.4 | _(include UC-002)_ | SD-UC-001-S01.md | 3. moveForwardWithCleaning() | MovementController | +moveForwardWithCleaning(): void | RVC | MovementController.moveForwardWithCleaning |
| UC-001 | UC-001-S01 | FR-002, §0.4 | _(include UC-002)_ | SD-UC-001-S01.md | 4. syncForwardCleaningState() | CleaningService | +syncForwardCleaningState(): void | RVC, CleaningOutput | CleaningService.syncForwardCleaningState |
| UC-001 | UC-001-S91 | FR-003, NFR-003 | handleObstacleDetected | SD-UC-001-S91.md | 1. handleObstacleDetected() | RvcController | +handleObstacleDetected(): void | RVC, Obstacle | RvcController.handleObstacleDetected |
| UC-002 | UC-002-S01 | FR-002, §0.4 | moveForwardWithCleaning | SD-UC-002-S01.md | 1. moveForwardWithCleaning() | MovementController | +moveForwardWithCleaning(): void | RVC | MovementController.moveForwardWithCleaning |
| UC-002 | UC-002-S01 | FR-002, §0.4 | moveForwardWithCleaning | SD-UC-002-S01.md | 2. syncForwardCleaningState() | CleaningService | +syncForwardCleaningState(): void | RVC, CleaningOutput | CleaningService.syncForwardCleaningState |
| UC-003 | UC-003-S01 | FR-003, NFR-003 | handleObstacleDetected | SD-UC-003-S01.md | 1. handleObstacleDetected() | RvcController | +handleObstacleDetected(): void | RVC, Obstacle | RvcController.handleObstacleDetected |
| UC-003 | UC-003-S01 | FR-003, §0.4 | stopCleaning | SD-UC-003-S01.md | 2. stopCleaning() | CleaningService | +stopCleaning(): void | CleaningOutput | CleaningService.stopCleaning |
| UC-003 | UC-003-S01 | FR-003, UR-001 | canTurnRight | SD-UC-003-S01.md | 3. canTurnRight() | MovementController | +canTurnRight(): bool | RVC, Obstacle | MovementController.canTurnRight |
| UC-003 | UC-003-S01 | FR-003, NFR-003 | canTurnRight | SD-UC-003-S01.md | 4. isRightTurnFeasible() | IObstacleDetector | +isRightTurnFeasible(): bool | Obstacle | IObstacleDetector.isRightTurnFeasible |
| UC-003 | UC-003-S01 | FR-003, UR-001 | turnRight | SD-UC-003-S01.md | 5. turnRight() | MovementController | +turnRight(): void | RVC | MovementController.turnRight |
| UC-003 | UC-003-S01 | FR-003, §0.4 | resumeForwardWithCleaning | SD-UC-003-S01.md | 6. resumeForwardWithCleaning() | MovementController | +resumeForwardWithCleaning(): void | RVC | MovementController.resumeForwardWithCleaning |
| UC-003 | UC-003-S01 | FR-003, §0.4 | resumeForwardWithCleaning | SD-UC-003-S01.md | 7. syncForwardCleaningState() | CleaningService | +syncForwardCleaningState(): void | RVC, CleaningOutput | CleaningService.syncForwardCleaningState |
| UC-003 | UC-003-S02 | FR-003, NFR-003 | handleObstacleDetected | SD-UC-003-S02.md | 1. handleObstacleDetected() | RvcController | +handleObstacleDetected(): void | RVC, Obstacle | RvcController.handleObstacleDetected |
| UC-003 | UC-003-S02 | FR-003, §0.4 | stopCleaning | SD-UC-003-S02.md | 2. stopCleaning() | CleaningService | +stopCleaning(): void | CleaningOutput | CleaningService.stopCleaning |
| UC-003 | UC-003-S02 | FR-003, UR-001 | canTurnRight | SD-UC-003-S02.md | 3. canTurnRight() | MovementController | +canTurnRight(): bool | RVC, Obstacle | MovementController.canTurnRight |
| UC-003 | UC-003-S02 | FR-003, NFR-003 | canTurnRight | SD-UC-003-S02.md | 4. isRightTurnFeasible() | IObstacleDetector | +isRightTurnFeasible(): bool | Obstacle | IObstacleDetector.isRightTurnFeasible |
| UC-003 | UC-003-S02 | FR-003, UR-001 | turnLeft | SD-UC-003-S02.md | 5. turnLeft() | MovementController | +turnLeft(): void | RVC | MovementController.turnLeft |
| UC-003 | UC-003-S02 | FR-003, §0.4 | resumeForwardWithCleaning | SD-UC-003-S02.md | 6. resumeForwardWithCleaning() | MovementController | +resumeForwardWithCleaning(): void | RVC | MovementController.resumeForwardWithCleaning |
| UC-003 | UC-003-S02 | FR-003, §0.4 | resumeForwardWithCleaning | SD-UC-003-S02.md | 7. syncForwardCleaningState() | CleaningService | +syncForwardCleaningState(): void | RVC, CleaningOutput | CleaningService.syncForwardCleaningState |
| UC-003 | UC-003-S91 | FR-004, NFR-003 | handleSurroundedObstacle | SD-UC-003-S91.md | 1. handleSurroundedObstacle() | RvcController | +handleSurroundedObstacle(): void | RVC, Obstacle | RvcController.handleSurroundedObstacle |
| UC-004 | UC-004-S01 | FR-004, NFR-003 | handleSurroundedObstacle | SD-UC-004-S01.md | 1. handleSurroundedObstacle() | RvcController | +handleSurroundedObstacle(): void | RVC, Obstacle | RvcController.handleSurroundedObstacle |
| UC-004 | UC-004-S01 | FR-004, §0.4 | stopCleaning | SD-UC-004-S01.md | 2. stopCleaning() | CleaningService | +stopCleaning(): void | CleaningOutput | CleaningService.stopCleaning |
| UC-004 | UC-004-S01 | FR-004, §0.4 | moveBackward | SD-UC-004-S01.md | 3. moveBackward() | MovementController | +moveBackward(): void | RVC | MovementController.moveBackward |
| UC-004 | UC-004-S01 | FR-004, UR-001 | canTurnRight | SD-UC-004-S01.md | 4. canTurnRight() | MovementController | +canTurnRight(): bool | RVC, Obstacle | MovementController.canTurnRight |
| UC-004 | UC-004-S01 | FR-004, NFR-003 | canTurnRight | SD-UC-004-S01.md | 5. isRightTurnFeasible() | IObstacleDetector | +isRightTurnFeasible(): bool | Obstacle | IObstacleDetector.isRightTurnFeasible |
| UC-004 | UC-004-S01 | FR-004, UR-001 | turnRight | SD-UC-004-S01.md | 6. turnRight() | MovementController | +turnRight(): void | RVC | MovementController.turnRight |
| UC-004 | UC-004-S01 | FR-004, UR-002, §0.4 | resumeForwardWithCleaning | SD-UC-004-S01.md | 7. resumeForwardWithCleaning() | MovementController | +resumeForwardWithCleaning(): void | RVC | MovementController.resumeForwardWithCleaning |
| UC-004 | UC-004-S01 | FR-004, §0.4 | resumeForwardWithCleaning | SD-UC-004-S01.md | 8. syncForwardCleaningState() | CleaningService | +syncForwardCleaningState(): void | RVC, CleaningOutput | CleaningService.syncForwardCleaningState |
| UC-004 | UC-004-S02 | FR-004, NFR-003 | handleSurroundedObstacle | SD-UC-004-S02.md | 1. handleSurroundedObstacle() | RvcController | +handleSurroundedObstacle(): void | RVC, Obstacle | RvcController.handleSurroundedObstacle |
| UC-004 | UC-004-S02 | FR-004, §0.4 | stopCleaning | SD-UC-004-S02.md | 2. stopCleaning() | CleaningService | +stopCleaning(): void | CleaningOutput | CleaningService.stopCleaning |
| UC-004 | UC-004-S02 | FR-004, §0.4 | moveBackward | SD-UC-004-S02.md | 3. moveBackward() | MovementController | +moveBackward(): void | RVC | MovementController.moveBackward |
| UC-004 | UC-004-S02 | FR-004, UR-001 | canTurnRight | SD-UC-004-S02.md | 4. canTurnRight() | MovementController | +canTurnRight(): bool | RVC, Obstacle | MovementController.canTurnRight |
| UC-004 | UC-004-S02 | FR-004, NFR-003 | canTurnRight | SD-UC-004-S02.md | 5. isRightTurnFeasible() | IObstacleDetector | +isRightTurnFeasible(): bool | Obstacle | IObstacleDetector.isRightTurnFeasible |
| UC-004 | UC-004-S02 | FR-004, UR-001 | turnLeft | SD-UC-004-S02.md | 6. turnLeft() | MovementController | +turnLeft(): void | RVC | MovementController.turnLeft |
| UC-004 | UC-004-S02 | FR-004, UR-002, §0.4 | resumeForwardWithCleaning | SD-UC-004-S02.md | 7. resumeForwardWithCleaning() | MovementController | +resumeForwardWithCleaning(): void | RVC | MovementController.resumeForwardWithCleaning |
| UC-004 | UC-004-S02 | FR-004, §0.4 | resumeForwardWithCleaning | SD-UC-004-S02.md | 8. syncForwardCleaningState() | CleaningService | +syncForwardCleaningState(): void | RVC, CleaningOutput | CleaningService.syncForwardCleaningState |
| UC-005 | UC-005-S01 | FR-005, NFR-003 | handleDustDetected | SD-UC-005-S01.md | 1. handleDustDetected() | RvcController | +handleDustDetected(): void | RVC, Dust | RvcController.handleDustDetected |
| UC-005 | UC-005-S01 | FR-005, NFR-003 | dustDetected | SD-UC-005-S01.md | 2. isDustPresent() | IDustDetector | +isDustPresent(): bool | Dust | IDustDetector.isDustPresent |
| UC-005 | UC-005-S01 | FR-005, UR-003, NFR-004 | boostCleaningPower | SD-UC-005-S01.md | 3. boostCleaningPower(durationSec=3) | CleaningService | +boostCleaningPower(durationSec: int): void | CleaningOutput | CleaningService.boostCleaningPower |
| UC-005 | UC-005-S01 | FR-005, NFR-004 | restoreDefaultCleaningPower | SD-UC-005-S01.md | 4. restoreDefaultCleaningPower() | CleaningService | +restoreDefaultCleaningPower(): void | CleaningOutput | CleaningService.restoreDefaultCleaningPower |
| UC-005 | UC-005-S01 | FR-002, §0.4 | moveForwardWithCleaning | SD-UC-005-S01.md | 5. moveForwardWithCleaning() | MovementController | +moveForwardWithCleaning(): void | RVC | MovementController.moveForwardWithCleaning |
| UC-005 | UC-005-S01 | FR-002, §0.4 | moveForwardWithCleaning | SD-UC-005-S01.md | 6. syncForwardCleaningState() | CleaningService | +syncForwardCleaningState(): void | RVC, CleaningOutput | CleaningService.syncForwardCleaningState |

## 3. 커버리지 요약

| 항목 | 건수 | 누락 |
|------|------|------|
| UC 시나리오(S01+) | 8 | 0 |
| SSD System Operation (고유) | 12 | 0 |
| SD 파일 | 8 | 0 |
| DCD Class.Method (고유) | 18 | 0 |
| FR (현재 범위 FR-001–005) | 5 | 0 |
| NFR (관련 NFR-002–004) | 3 | 0 |

**고유 Code Symbol (18):** RvcController×4, MovementController×7, CleaningService×6, IObstacleDetector×1, IDustDetector×1

## 4. OOI handoff

- 구현: `include/rvc/`, `src/rvc/` — DCD 클래스 1:1
- `IObstacleDetector` / `IDustDetector` → OOI에서 `ObstacleDetector`, `DustDetector` 구현체
- SSD system operation → `RvcController` public API 진입점
