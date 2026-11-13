# GetPatched libgit2 recipe

This repository contains public recipes for backporting selected libgit2 security fixes to Ubuntu 20.04 Focal and Ubuntu 22.04 Jammy. The recipe pins Ubuntu's public source package and records target-specific patches, regression fixtures, and licensing evidence.

## Supported target

| Distribution | Architecture | Source version | Candidate version | Status |
| --- | --- | --- | --- | --- |
| Ubuntu Focal | amd64 | `0.28.4+dfsg.1-2ubuntu0.1` | `0.28.4+dfsg.1-2ubuntu0.1+getpatched1` | Hosted technical checks passed; candidate policy promotion and PR gates pending |
| Ubuntu Jammy | amd64 | `1.1.0+dfsg.1-4.1ubuntu0.1` | `1.1.0+dfsg.1-4.1ubuntu0.1+getpatched1` | Verification rerun required; no release eligibility |

Debian Bookworm and Trixie are excluded because their current public source packages include fixes for the three selected CVEs. Ubuntu Noble and Resolute are excluded because their public source packages already include the selected fixes. Only amd64 is declared; arm64 needs its own real build, scan, and clean-client verification. Both targets use public Ubuntu source packages and never use Ubuntu Pro or ESM source packages.

Each source descriptor and both source archives are pinned by SHA-256 in its target file and matched to the signed Ubuntu source index. The signed index authenticates the file checksums; individual `.dsc` uploader signatures have not been separately verified.

## Backports

- **CVE-2026-53584:** rejects submodule paths that traverse outside the repository. The patch validates paths during enumeration, lookup/reload, and configuration loading.
- **CVE-2026-53585:** limits declared object sizes while unpacking untrusted pack data. The limit is checked before applying a pack delta; ordinary binary patch application remains independent of the pack limit.
- **CVE-2026-53586:** passes the current redirected server URL to the credentials callback so applications can make the credential decision for the actual endpoint.

Each patch is target-scoped to its pinned Ubuntu source version, records its upstream commit and CVE in DEP-3 metadata, and has an associated regression fixture or test. The Focal hosted technical verification is run [41142564467](https://github.com/getpatched/platform/actions/runs/41142564467); it reports successful installed baseline/patched regressions, package-aware scans, and isolated signed APT checks, but `candidateVerified=false` under its unpromoted candidate policy. Historical Jammy run [41167536831](https://github.com/getpatched/platform/actions/runs/41167536831) used the wrong pack-size regression fixture and does not prove CVE-2026-53585. Corrected-fixture run [43349343782](https://github.com/getpatched/platform/actions/runs/43349343782) reproduced all three exact Jammy baseline issues and passed patched installed regressions, package-aware scans, build/package tests, compatibility, and isolated signed APT checks. However, its artifact incorrectly set `releaseEligible=true`, so it is not accepted as the campaign count basis; a corrected verification run is required. The current candidate verification is [run 43353250317](https://github.com/getpatched/platform/actions/runs/43353250317). For both targets, production publication is disabled; raw Trivy 0.70 reports the three CVEs against candidate packages and is not considered a clean scan. CVE-2026-53587 is excluded for Focal and Jammy: Canonical's [official record](https://ubuntu.com/security/CVE-2026-53587) marks both releases as not affected. CVE-2026-5917 is a separate issue whose Ubuntu fixes for Focal and Jammy are ESM-only; it is outside this selected three-CVE patch set and is not claimed fixed here.

The package version appends `+getpatched1` to the Ubuntu version. Additional patches on the same source increment the GetPatched serial; a different distro source version requires a separately pinned target and fresh verification.

## Verification and release

Before release, the hosted workflow must build the actual Ubuntu source and binary packages, reproduce each issue in the exact installed baseline, pass the patched installed regressions and consumer checks, compare fixed-database package-aware scans, and verify signed APT installation and source retrieval from a clean client. No binary is published by this development recipe. A passing release will be public immediately with no sponsor gate or delay.

## Licensing

The MIT license in `LICENSE` covers only GetPatched-authored recipe metadata, tests, and documentation. libgit2 and the adapted security patches retain upstream copyright and GPL licensing with the libgit2 linking exception, recorded in `LICENSES/upstream-COPYING`; Ubuntu packaging notices are preserved in `LICENSES/ubuntu-source-copyright`. Binary and source releases must preserve all upstream and distro license notices.
