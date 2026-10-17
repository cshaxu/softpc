# M9 T95 S2 Paused DMA Repair

Owner request: "批准收口S1和准入S2". Delivery 66055d92fa464933ddf442dc50fff4d880093f80
was reviewed after push in 6c3b604fa6df3c7979285df2a94428f516a30033.
Owner subsequently delegates closure: "我没空收口。你帮我收口准入下一个t任务吧".

The existing DspServiceTransfer entry honors bDspPaused before submitting pending
bytes. Completion callbacks, IRQ masks and snapshot layout are unchanged.
Production +2/-1 net +1; existing test +12/-0 net +12. No public code changes.
Full evidence and similar-issue sweep are in the
[retained proposal](M9-T95-win31-midi-wav-playback-proposal.md#s2-delivery-audit).

Both widths have red/green proof, 193/193 full tests, four WAVs then twelve
Chord plays, Windows exit/restart, and full MPU Canyon followed by WAV.
Native output is distinct from mock proof. Original Ad Lib configuration stays
unchanged; FM/SB-MIDI capability is not expanded and parser debt remains.
S2 closes under delegated authority, not a new manual-playback claim.
