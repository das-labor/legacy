also entwicklung hier laeuft wie folgt ab:

##################################################################################
a) erzeuge eine branch vom trunk im ordner branch

cd $project
svn cp trunk branch/mybranch
svn ci -m 'branch for blafoo'

##################################################################################
b) wechsle in deinen branch-ordner und aktualisiere um die externals zu bekommen

cd branch/mybranch
svn up

##################################################################################
c) hack the files
${EDITOR} file.c

##################################################################################
d) commit all changes

cd branch/mybranch
svn ci -m 'fixed stuff'

##################################################################################
e) wenn der branch laeuft und nur dann fuehre ihn mit dem trunk zusammen
wenn manche branches nicht lauf auch egal, hauptsache der trunk laeuft

cd trunk
svn merge https://www.das-labor.org/svn/autoLab/$project/branch/mybranch

##################################################################################
f) danach kann der branch geloesht werden

cd branch
svn del mybranch
svn ci -m 'branch done'



greets

Asklepios



