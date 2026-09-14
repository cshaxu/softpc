# M9 Td S15: overlay queue promotion

Owner request: “overlay 页查询 O(n) 这个加入一个队列proposal，td提交；然后所有其他todo，请你准入一个s任务予以处理”。

Baseline `3fd862a`. P1 `598e52f` delivered and pushed the
[overlay proposal](../proposals/m9-overlay-page-index.md).
Review of the committed diff confirms only overlay moved from TODO to queue
position three; XP and Win3.1 remain ahead, and seven other TODOs are unchanged.
Measurement precedes algorithm selection; no API/media/product change is claimed.

Documentation governance and diff hygiene pass; closure reruns the governance
self-test. No runtime code or EXE changed. Td closes without allocating a T;
the separately authorized remaining-debt task is admitted next as T59 S1.
