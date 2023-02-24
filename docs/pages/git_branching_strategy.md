# Git Branching Strategy

## History
 
|Version|Date|Author|Notes|
|-----------|------|-----|---|
|1.2|23rd Feb 2023|G.Weatherup|Updated for release, with reworked template|
|1.1|23th Nov 2022|G.Weatherup|Added useful information|
|1.0|23th Nov 2022|G.Weatherup|Initial release|

## Table of Contents

- [Overview](#overview)
    - [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
    - [References](#references)
- [Gitflow vs trunk-based development](#gitflow-vs-trunk-based-development)
- [Process on creating code changes](#process-on-creating-code-changes)
    - [Release Process](#release-process)
- [Useful Information](#useful-information)
 
## Overview
 
The purpose of this document is to describe the standard method of working for developers using `git`.

### Acronyms, Terms and Abbreviations

- `git` \- Git is a free and open source distributed version control system.
- `tig`  \- Tig is an ncurses-based text-mode interface for git.
- `git-flow`  \- Strategy for managing Git branches.
- `git-hub flow`  \- GitHub flow is a lightweight, branch-based workflow.
- `OEM`  \- Original Equipment Manufacture (Sky is an OEM)
- `vendor`  \- 3rd Party `Soc` or `Software provider` who will be required to pass all the tests
- `patching`  \- File applied independantly ontop of a software delivery.
- `pull request`  \- Github review, let's you tell others about changes you've pushed to a branch in a repository on GitHub

### References

- `Git SCM`  \- Source Code Management tool - [https://git-scm.com/]
- `doxygen`  \- Source Code documentation tool - [https://www.doxygen.nl/index.html]
- `gitflow`  \- [https://nvie.com/posts/a-successful-git-branching-model/]
- `github flow`  \- [https://docs.github.com/en/get-started/quickstart/github-flow]
- `Trunk based development`  \- [https://www.atlassian.com/continuous-delivery/continuous-integration/trunk-based-development]
- `Github Creating an issue or pull request`  - [https://docs.github.com/en/desktop/contributing-and-collaborating-using-github-desktop/working-with-your-remote-repository-on-github-or-github-enterprise/creating-an-issue-or-pull-request]
- `Tig Manual`  - [https://manpages.ubuntu.com/manpages/bionic/man1/tig.1.html#:~:text=Tig%20is%20an%20ncurses%2Dbased,output%20from%20various%20Git%20commands]
- `Using git-flow or not`  -  [https://skoch.github.io/Git-Workflow/without-gitflow.html]
- `auto-changelog`  - [https://www.npmjs.com/package/auto-changelog]

## Git-flow vs trunk-based development

There are many discussions for using git-flow compared to trunk-based development flows like 'git-hub flow'

Trunk-based development is simple, focuses on the main branch as the source of fixes and releases. In trunk-based development, the main branch is assumed to always be stable, without issues, and ready to deploy. Web apps are typically continuous delivery, not rolled back, and don't have to support multiple versions of software running in the wild.

Git-flow uses feature branches (and can be longer-lived branches) with multiple primary branches. It has seperate primary branch lines for development, hotfixes, support, features and releases.

In practice the use case for the `Vendor/OEM` delivery is focused on embedded software, it is not stable, and will have many issues, and is not ready for deployment. For choosing a branching model therefore the following requiremnets must be met :-

- Fixed tagged releases.
- Rollback of previous versions required, via release tags.
- Multiple parellel workstreams, components have independant lifecycles.
- Bug fixing previous releases, branching, and supporting older releases is required.

Therefore`git-flow` fits the requirements whereas `trunk-based` flow does not.

## Process on creating code changes

Git `branch` will be used as a temporary work location, whilst a change is being developed with the intent to merge the branch with the origin as soon as possible.

The following description uses the `git-flow` commands, if they're not available see :- [Git-flow command breakdown reference](#references)

The overall process of flow for an individual change or feature is:-

1) Raise a ticket in JIRA, describing in detail what is the requirement for change, the description should contain enough information that it makes clear what the task is required todo.

2) Create a branch based on that Jira ticket number + synopsis, e.g. RDKB_45633_add32Tests

Init of flow is required once per clone for the git repo.

```bash
git flow init -d
```

Starting a branch:-

```bash
git flow feature start RDKB_45633_add32Tests
```

the branch will be labeled as `feature/RDKB_45633_add32Tests`

3) Perform the changes, adding the code as required into the branch, and check what `tig` can offer, but the git commands are :-

```bash
git add <file>
git commit
git push -u
```
First time, to set the uplink, git will report on the command line the link for the uplink, you then run that command. `git push` - all other times, after the uplink is set is all that is required.

*Testing Suites: Ideally a testing suite is available for the component. if present, then all tests must pass before the component is sent for review*

4) Generate pull requests in github, add the component owners into the review, and any other team members as required.

On the command line, when a new branch is pushed, github will report back in a link to create a `pull request` use this link to generate a `review` for the code or changes developed.

Alternatively you can follow the guide linked [Github Creating an issue or pull request](https://docs.github.com/en/desktop/contributing-and-collaborating-using-github-desktop/working-with-your-remote-repository-on-github-or-github-enterprise/creating-an-issue-or-pull-request)

5) The `Code Review Team` reviews the pull request and applies comments as required.
6) The engineer changes the code, commits and sends it back for review.
7) Reviewers are happy with the review and approve the merge.
8) The engineer can now merge branch, either from the UI or with :

```bash
git flow feature finish
```

Release if required, otherwise, repeat from (1), until which point you're ready to combine into an official release.
 
### Release Process
 
To release when required, you will will need to contact the release team for the module.
 
```bash
git flow release start <version>
```

where version is <major.minor.bugfix> ( Major change shouldn’t happen unless there’s an interface definition change )

Re-generate the change log, ideally auto-generated via `auto-changelog`

```bash
auto-changelog -v <version>
git add CHANGELOG.md
git commit CHANGELOG.md
git flow release finish
```

Assuming no merge conflicts, you should be now on `develop`, if you have merge conflicts then these will be required to be resolved.

```bash
git push
git checkout master
git push
git push --tags
```

## Useful Information
 
#### Configuration to auto uplink
 
```bash
git config --global --add --bool push.autoSetupRemote true
```
 
Stops you needing to perform git push -u, then following the git information returned, it auto generates the uplink when you perform git push.
 
#### Change your PS1 so you can see what branch you are on
 
Recommended to show you what branch you're sitting on, in your linx command prompt.
 
Add the following code to your .bashrc :
 
```bash
# Get GIT branch (if any)
function parse_git_branch ()
{
    git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/ (\1)/'
}
 
# Various colours
RED="\[\033[0;31m\]"
YELLOW="\[\033[0;33m\]"
GREEN="\[\033[0;32m\]"
CYAN="\[\033[0;36m\]"
NO_COLOR="\[\033[0m\]"
 
PS1="$GREEN\u@\h$NO_COLOR \W$CYAN\$(parse_git_branch)$NO_COLOR\$ "
```