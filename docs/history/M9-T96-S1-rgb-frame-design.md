# T96 S1 RGB Frame Design

Original requests: "批准开始" and "把这个无份清理的任务排到本t的最后一个s任务 然后接着做现在的升级".
Design delivery 7d0d3dea is pushed and actual-diff reviewed against the active
read-only packet. Only proposal/CURRENT changed: production/test +0/-0.
Both-width independent layout/allocation probe passes; no runtime qualification
or new EXEs claimed. Five existing heap frame payloads grow by 14740480 bytes.
Owner directs buffer cleanup to final S4 and continuation of the RGB upgrade.

The retained proposal defines three expected production paths, nine test paths,
atomic Lib/SoftPC migration and finite coverage. No new alias, compatibility
format, original-mirror edit or private palette removal. No unsupported game
claim. Documentation gate and actual diff review pass. S1 closes under the
owner's continuation instruction; S2 is admitted in CURRENT. S3 audits adoption;
S4 simplifies buffers before whole-T closure. Owner inputs remain unchanged.
