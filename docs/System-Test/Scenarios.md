# System Test Scenarios



`System-Test/scenarios/ST-*.json` — **11건** (맵 5종, 중복 패턴 통합)



## 맵 템플릿



| 맵 | 크기 | 사용 시나리오 | 특징 |

|----|------|---------------|------|

| Apartment | 10×10 | ST-001, ST-007~010 | 가구·벽 클러스터, 복도·장애물·먼지·삼방향 막힘 |

| Room8 | 8×8 | ST-003, ST-004 | 모서리 장애물 + 전방 1개 장애물 회피 |

| Pocket8 | 8×8 | ST-005, ST-006 | 측면 벽 + 중앙 삼방향 막힘 포켓 |

| DustRoom | 8×8 | ST-002 | 네 기둥 + 중앙 먼지 |

| Corridor12 | 12×6 | ST-011 | 측벽 복도 + 중간 장애물 |



## 시나리오 목록



| ID | 이름 | trigger | ticks | FR | Capabilities (≥3) |

|----|------|---------|-------|-----|-------------------|

| ST-001 | Start forward north in apartment hallway | start | 3 | FR-001, FR-002 | CAP-001, CAP-002, CAP-012 |

| ST-002 | Dust trigger boost and forward | dust | 1 | FR-005, UR-003 | CAP-009, CAP-010 |

| ST-003 | Obstacle avoid turn right resume | obstacle | 3 | FR-003, UR-001 | CAP-003, CAP-004, CAP-005, CAP-007 |

| ST-004 | Obstacle avoid turn left resume | obstacle | 3 | FR-003, UR-001 | CAP-003, CAP-004, CAP-006, CAP-007 |

| ST-005 | Surrounded backward turn right resume | surrounded | 4 | FR-004, UR-001, UR-002 | CAP-003, CAP-008, CAP-005, CAP-007 |

| ST-006 | Surrounded backward turn left resume | surrounded | 4 | FR-004, UR-001, UR-002 | CAP-003, CAP-008, CAP-006, CAP-007 |

| ST-007 | Auto forward then obstacle turn east | start | 5 | FR-002, FR-003 | CAP-001, CAP-002, CAP-003, CAP-005, CAP-007 |

| ST-008 | Auto path with dust boost mid-run | start | 4 | FR-002, FR-005 | CAP-001, CAP-002, CAP-009, CAP-010 |

| ST-009 | Auto obstacle avoid then continue east | start | 6 | FR-002, FR-003 | CAP-001, CAP-002, CAP-003, CAP-005 |

| ST-010 | Auto tick when L/R/F blocked must rotate | start | 5 | FR-004, UR-001 | CAP-003, CAP-008, CAP-006, CAP-007 |

| ST-011 | Long east corridor with side walls | start | 6 | FR-002 | CAP-001, CAP-002, CAP-012 |



## GUI 시뮬레이터 조작



| 키 | 동작 |

|----|------|

| S | startAutomaticCleaning (첫 tick = 세션 시작 + 1동작) |

| T | tick 1회 (전진·후진·회전 중 **하나**만) |

| R | 시나리오 ticks 실행 |

| [ / ] | manifest 시나리오 이전 / 다음 선택 |

| L | 선택한 ST 시나리오 로드 |

| F5 | custom_map.json 저장 (`map=custom`) |

| C | 디버그 로그 클립보드 복사 |

| F6 | 디버그 로그 `debug_log.txt` 저장 |

| E | 편집 모드 토글 |

| O / D | 장애물 / 먼지 배치 |

| N | 기본 맵 리셋 |

### 디버그 로그

- **L** 로드 → `map=ST-001` 등 manifest ID
- **F5** 저장 / 그리드 편집 → `map=custom`
- **N** 리셋 → `map=SIM-DEFAULT`
- tick마다 `move`, `pos`, `dir`, `clean`, `session`, `maneuver`, `front_blk`, `surrounded` 한 줄 기록
- **C** 클립보드 복사, **F6** → `System-Test/scenarios/debug_log.txt` 저장
- 터미널에서 실행 시 stdout에도 동일 출력

편집 모드에서 그리드 클릭으로 obstacle·dust 배치. 경로는 tick마다 실시간 표시.



## 검증



`rvc_system_test`가 manifest의 JSON을 로드하고 `expectedPath`와 실제 `MapModel.path()`를 비교한다.  

실패 시 stderr에 actual path가 출력된다.

