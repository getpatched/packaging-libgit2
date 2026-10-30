# GetPatched libgit2 recipe

This repository contains the public recipe for backporting libgit2 security fixes to Ubuntu 20.04 Focal. The recipe pins Ubuntu's public source package and records target-specific patches, regression fixtures, and licensing evidence.

## Supported target

| Distribution | Architecture | Source version | Candidate version | Status |
| --- | --- | --- | --- | --- |
| Ubuntu Focal | amd64 | `0.28.4+dfsg.1-2ubuntu0.1` | `0.28.4+dfsg.1-2ubuntu0.1+getpatched1` | Unverified development candidate |

Debian Bookworm and Trixie are excluded because their current public source packages include fixes for the requested libgit2 issues. Ubuntu releases other than Focal and arm64 are not declared supported here. This target uses the latest public Focal source package; no Ubuntu Pro or ESM package is used.

The source descriptor and both source archives are pinned by SHA-256 in `targets/ubuntu/focal-amd64.json`, matching the signed Ubuntu source index. The signed index authenticates the file checksums; the individual `.dsc` uploader signature has not been separately verified.

## Backports

- **CVE-2026-53584:** rejects submodule paths that traverse outside the repository. The patch validates paths during enumeration, lookup/reload, and configuration loading.
- **CVE-2026-53585:** limits declared object sizes while unpacking untrusted pack data. The limit is checked before applying a pack delta; ordinary binary patch application remains independent of the pack limit.
- **CVE-2026-53586:** passes the current redirected server URL to the credentials callback so applications can make the credential decision for the actual endpoint.

Each patch is version scoped to the pinned Focal source, records its upstream commit and CVE in DEP-3 metadata, and has an associated regression fixture or test. **The patches and installed-package checks remain unverified; this repository does not claim the CVEs are fixed yet.** CVE-2026-53587 is excluded because the audited Focal implementation does not contain the affected object-format code.

The package version appends `+getpatched1` to the Ubuntu version. Additional patches on the same source increment the GetPatched serial; a different distro source version requires a separately pinned target and fresh verification.

## Verification and release

Before release, the hosted workflow must build the actual Ubuntu source and binary packages, reproduce each issue in the exact installed baseline, pass the patched installed regressions and consumer checks, compare fixed-database package-aware scans, and verify signed APT installation and source retrieval from a clean client. No binary is published by this development recipe. A passing release will be public immediately with no sponsor gate or delay.

## Licensing

The MIT license in `LICENSE` covers only GetPatched-authored recipe metadata, tests, and documentation. libgit2 and the adapted security patches retain upstream copyright and GPL licensing with the libgit2 linking exception, recorded in `LICENSES/upstream-COPYING`; Ubuntu packaging notices are preserved in `LICENSES/ubuntu-source-copyright`. Binary and source releases must preserve all upstream and distro license notices.
