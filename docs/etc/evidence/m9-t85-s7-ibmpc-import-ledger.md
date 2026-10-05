# T85 S7 IBM PC import ledger

Frozen references: SoftPC 45a461a2 and read-only NXVM
ebdb40098dfaa7b2bcc0f294d52e3f19dafd6629. This is import/ownership evidence,
not manual acceptance or an S/T closure.

## Finite universe and disposition

All 126 src/ibmpc paths are byte-identical to frozen upstream; its existing
manifest records every member. IBM PC is not wired into SoftPC's runtime.

Upstream has 255 test paths; the retained manifest names each local member.
Test edits adjust local includes/source ownership, keep assertions active in
Release and use existing Types vocabulary. Seven additional inner fixture
headers are local support, not duplicate CPU programs or production executors.
No test package borrows another package's C/H.

Protected IRET is registered once by test/x86/core/machine_protected_iret_smoke.c.
IBM PC core_machine_iret_s51_smoke.c keeps real-mode/PIC assertions but removes
the repeated protected-IRET include and invocation. No full inner CPU program
is copied into this suite.

## All 51 prior exclusions

Former test/x86 paths below move to the same relative test/ibmpc path. All
45 retained members are reachable from generated CMake DependInfo sources and
recursive quoted includes. Six omitted helpers have no remaining consumer.

- `chips/cpu/machine_idt_privilege_pic_board_smoke.c`: restored; registered-source/include closure verified.
- `chips/cpu/machine_outer_iret_pic_board_smoke.c`: restored; registered-source/include closure verified.
- `chips/cpu/machine_protected_data_pic_board_smoke.c`: restored; registered-source/include closure verified.
- `chips/cpu/machine_protected_far_pic_board_smoke.c`: restored; registered-source/include closure verified.
- `chips/cpu/machine_task_switch16_pic_board_smoke.c`: restored; registered-source/include closure verified.
- `chips/cpu/support/protected_pic_board_fixture.h`: restored; registered-source/include closure verified.
- `core/construction_fixture.h`: restored; registered-source/include closure verified.
- `core/core_machine_ega_registration_transaction_smoke.c`: restored; registered-source/include closure verified.
- `core/core_machine_fpu_8087_smoke.c`: restored; registered-source/include closure verified.
- `core/core_machine_pic_phase_s2_smoke.c`: restored; registered-source/include closure verified.
- `core/core_machine_ram_create_smoke.c`: restored; registered-source/include closure verified.
- `core/core_machine_rom_route_transaction_smoke.c`: restored; registered-source/include closure verified.
- `core/dma_route_rollback_smoke.c`: restored; registered-source/include closure verified.
- `core/kbc_controller_fixture.c`: restored; registered-source/include closure verified.
- `core/machine_board_binding_identity_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_checked_memory_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_competition_80386_s1_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_cpu_reset_identity_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_explicit_time_s4_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_firmware_capability_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_fpu_interface_s65_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_instance_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_instruction_timing_ledger_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_legacy_timing_normalization_s2_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_reset_rom_alias_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_retirement_observation_s3_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_scheduler_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_t359_s2_timing_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_t359_s3_timing_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_timeline_s2_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_transaction_lifecycle_s4_smoke.c`: restored; registered-source/include closure verified.
- `core/machine_transaction_s2_smoke.c`: restored; registered-source/include closure verified.
- `core/plan_core_fixture.c`: restored; registered-source/include closure verified.
- `core/plan_core_fixture.h`: restored; registered-source/include closure verified.
- `core/planar_parity_fixture.c`: restored; registered-source/include closure verified.
- `core/planar_parity_fixture.h`: restored; registered-source/include closure verified.
- `core/port_assembly_core_smoke.c`: restored; registered-source/include closure verified.
- `core/port_assembly_fixture.c`: restored; registered-source/include closure verified.
- `core/port_assembly_fixture.h`: restored; registered-source/include closure verified.
- `core/xt_ppi_controller_fixture.c`: restored; registered-source/include closure verified.
- `core/boot_fixture.c`: omitted; no remaining consumer.
- `core/boot_fixture.h`: omitted; no remaining consumer.
- `core/bus_fixture.h`: restored; registered-source/include closure verified.
- `core/composition_fixture.c`: restored; registered-source/include closure verified.
- `core/composition_fixture.h`: restored; registered-source/include closure verified.
- `core/memory_registration_fixture.c`: omitted; no remaining consumer.
- `core/memory_registration_fixture.h`: omitted; no remaining consumer.
- `core/time_fixture.c`: restored; registered-source/include closure verified.
- `core/time_fixture.h`: restored; registered-source/include closure verified.
- `core/video_topology_fixture.c`: omitted; no remaining consumer.
- `core/video_topology_fixture.h`: omitted; no remaining consumer.

## Upstream IBM PC exclusions

These 38 paths are not registered by upstream test/ibmpc/CMakeLists.txt.
App cases require product profiles absent from the shared package. The two
timing qualification runners embed whole inner CPU programs. They remain in
NXVM; this import claims neither their product nor timing qualification.
NXVM App/IBM PC files are never modified.

- `board-common/composition/core_machine_port_assembly_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `board-common/composition/machine_80286_timing_manifest_runner.c`: unregistered timing qualification tool; upstream owner retained.
- `board-common/composition/machine_80386_timing_manifest_runner.c`: unregistered timing qualification tool; upstream owner retained.
- `board-common/composition/machine_competition_s3_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `board-common/composition/machine_time_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/nxvm_machine_reconfigure_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/nxvm_machine_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_boot_failure_lifecycle_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_cga_graphics_system_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_cmos_rtc_port_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_console_pause_resume_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_core_executor_storage_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_display_composition_s5_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_ega_controller_system_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_ega_sequencer_system_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_fault_outcome_runner_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_fdc_authority_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_fdc_port_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_fdc_t242_corpus_port_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_hdc_port_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_host_cancellation_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_ibm_5170_direct_plan_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_kbc_aux_guest_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_keyboard_host_ingress_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_machine_initialization_atomicity_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_machine_media_lifecycle_s3_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_machine_speed_policy_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_model_339_clock_contract_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_pcat_composition_s4_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_pcat_ownership_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_pcat_topology_s2_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_runner_display_cadence_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_runner_error_propagation_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_timing_qualification_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_two_session_isolation_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/composition/vm_x86_debug_mapping_smoke.c`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/support/profile.h`: unregistered App-profile integration or dependent fixture; upstream owner retained.
- `machine/support/rom/session_assets.h`: unregistered App-profile integration or dependent fixture; upstream owner retained.

## Verification

Independent Release IBM PC build/test passes 161/161 on x64 (115.07 seconds).
Only the IBM PC suite is configured; inner production targets, not their test
packages, supply its dependencies. Generated source/include closure has 208
reachable local C/H members and no foreign suite or product input.
Static source DAG, four suite boundaries and Types positive/negative probes
pass. Both product Release builds pass; final hidden-background CTest passes
x64 443/443 (161.57s) and x86 443/443 (237.47s). Five desktop cases per width
are excluded. No Linux runtime or IBM PC product boot is claimed. All eight
manifests pass; IBM PC's 126 paths still match frozen upstream exactly.

Staged source/test/build numstat against 45a461a2: production C/H +13,396/-0
(117 paths), tests C/H +50,074/-4 (265 paths), build/check/attributes +1,113/-6
(18 paths). Combined +64,583/-10, net +64,573. Documentation/manifests and EXEs
are counted separately. These are imports, not newly invented product logic.
Within retained upstream IBM PC C/H files, local adaptation is +812/-874,
net -62 across 142 paths; the 45 returning files and seven support headers are
separate from that retained-file comparison. All 126 production files match.

The first broad registration accidentally selected both normal and observation
Core variants; removing the unnecessary normal-Core link preserves upstream's
explicit variant choice. Initial root generation overlapped that edit and
retained stale flags, so it failed before delivery. Regenerated flags select
only CORE_MACHINE_RUNTIME_TRACE_ENABLED=1 for observation cases. No warning
disable or runtime recovery branch was added. Final results below supersede
those failed build attempts, not the underlying functional assertions.

The expanded Types checker found four direct exit calls in three existing
x86 test files. They now use the same existing Release-enabled assertion as
IBM PC fixtures: +5/-4, net +1. No Lib production/API change is needed.
The failed initial x64 checker run is retained as discovery evidence; the
affected x86 tests are rebuilt before final full-suite verification.

An interrupted earlier x64 run left deliberately invalid x86 verifier probe
files. The next x64 sweep passed 442/443, rejecting the stale unrelated probe
instead of the case under test. Process inspection confirmed no remaining
old SoftPC test writer; only the known generated stale probe was removed.
A new absolute-root independent negative sweep passed, then the complete
x64 background suite passed 443/443. The x86 final suite also passes 443/443.
No verifier expectation, functional assertion or timeout was weakened.
These failed/interrupted attempts are not counted as successful qualification.

Final product linkLibs.rsp on both widths contains existing SoftPC/Common/Lib
and Debug/xasm32 archives, not IBM PC or shared x86 Core. PE widths are
0x14c and 0x8664, with 3,505,512 and 3,098,361 bytes respectively. SHA-256:
x86 68C7318C0A43682AB44E745721ACBEB4C55C72B4E27C48F4C9265B21B03A960D;
x64 599651F6E95F9A8F05131FCE5E552DCD2FBCC9EC24DF3D3E9D75EA5F35DF3E71.
Both are rebuilt; x64 bytes remain identical to the baseline. Owner INI and
snapshot changes stay outside delivery. Owned scratch logs/builds are removed
after this evidence summary; reusable root configurations remain.

## Actual-change delivery review

Executor P1 8a967acf is committed and pushed. The coordinator reviewed its
actual 413 paths against the original owner request and packet: 126 source
paths, 269 test paths and 18 existing build/check/test/document/artifact paths.
Existing src/lib, src/common, src/x86 and src/app-softpc have no changed path.
Three x86 test files change only assertion vocabulary; shared tools add the
fourth inward layer and exit/abort probes. IBM PC production matches frozen
NXVM and does not become SoftPC's execution backend. All 51 former exclusions
and 38 omitted upstream paths have explicit dispositions above.
Counts, full-suite results, link inputs, PE widths and artifact hashes agree
with the actual delivery. No tests borrow another test suite; no App-profile
or runtime external-checkout dependency is imported. Owned scratch has been
removed, and all task-owned changes are committed; the two preexisting owner
INI/snapshot edits remain untouched. Await owner testing; neither S7 nor T85
is closed by this review.
