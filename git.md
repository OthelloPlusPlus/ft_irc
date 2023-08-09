# Creation
## Create new git
git init

# Ignore
## Ignore File
<!-- command + shift + . -->
.git/info/exclude
## Ingore Rule
echo "[rule]" >> .git/info/exclude

# Information
## Local
git status
## Remote
git ls-tree --full-tree -r --name-only HEAD

# Prepare to push
git add [filename]
(git rm --cached [filename])
git commit -m [message]

# Remote git
## Link git to existing git
git remote add origin [git_repository_link]
## Push remote
git push origin master
## clone
git clone [git repository link] [name]

# Branches
## Create and Swap branch
git checkout -b [branch]
git push origin [branch]
git push --set-upstream origin [branch]
## Swap branch
git checkout [branch]
## Work on branch
git pull
<!-- do stuff -->
git push
## Merge
requires clean tree

git checkout [master]
git merge [branch]

# Hyperlinks
https://www.nobledesktop.com/learn/git/git-branches
