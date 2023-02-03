(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ nohup python3 dj.py 16 WBDD 0 >> out.txt &
[1] 9790
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ nohup: ignoring input and redirecting stderr to stdout

(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ cat out.txt 
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ ps ax | grep run
    500 ?        Ss     0:00 /usr/bin/python3 /usr/bin/networkd-dispatcher --run-startup-triggers
    677 ?        Ss     0:00 sshd: /usr/sbin/sshd -D -o AuthorizedKeysCommand /usr/share/ec2-instance-connect/eic_run_authorized_keys %u %f -o AuthorizedKeysCommandUser ec2-instance-connect [listener] 0 of 10-100 startups
   9797 pts/0    S+     0:00 grep --color=auto run
[1]+  Done                    nohup python3 dj.py 16 WBDD 0 >> out.txt
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ cat out.txt 
Correct , time:  4.191705942153931  iter_count:  0
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ nohup python3 dj.py 32 WBDD 0 >> out.txt &
[1] 9800
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ nohup: ignoring input and redirecting stderr to stdout

(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ cat out.txt 
Correct , time:  4.191705942153931  iter_count:  0
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ ps ax | grep run
    500 ?        Ss     0:00 /usr/bin/python3 /usr/bin/networkd-dispatcher --run-startup-triggers
    677 ?        Ss     0:00 sshd: /usr/sbin/sshd -D -o AuthorizedKeysCommand /usr/share/ec2-instance-connect/eic_run_authorized_keys %u %f -o AuthorizedKeysCommandUser ec2-instance-connect [listener] 0 of 10-100 startups
   9803 pts/0    S+     0:00 grep --color=auto run
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ ps ax | grep pythioin
   9805 pts/0    S+     0:00 grep --color=auto pythioin
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ ps ax | grep python
    500 ?        Ss     0:00 /usr/bin/python3 /usr/bin/networkd-dispatcher --run-startup-triggers
    556 ?        Ssl    0:00 /usr/bin/python3 /usr/share/unattended-upgrades/unattended-upgrade-shutdown --wait-for-signal
   9800 pts/0    R      0:30 python3 dj.py 32 WBDD 0
   9807 pts/0    S+     0:00 grep --color=auto python
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ ps ax | grep pythioin


^[[A^[[B^[[B^[[A^[[A^[[A^[[B   9811 pts/0    S+     0:00 grep --color=auto pythioin
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ 
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ 
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ 

(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ 
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ ps ax | grep python
^C^Z

(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ 
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ 


(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ 
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ 
(tool) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ client_loop: send disconnect: Broken pipe
(base) meghana@wireless-10-145-15-239 python_pkg % 
(base) meghana@wireless-10-145-15-239 python_pkg % 
(base) meghana@wireless-10-145-15-239 python_pkg % ssh -i "cflobdd.cer" ubuntu@ec2-3-87-108-137.compute-1.amazonaws.com
Warning: Identity file cflobdd.cer not accessible: No such file or directory.
Welcome to Ubuntu 22.04.1 LTS (GNU/Linux 5.15.0-1028-aws x86_64)

 * Documentation:  https://help.ubuntu.com
 * Management:     https://landscape.canonical.com
 * Support:        https://ubuntu.com/advantage

  System information as of Fri Feb  3 05:52:02 UTC 2023

  System load:  0.0                Processes:             122
  Usage of /:   13.6% of 28.89GB   Users logged in:       0
  Memory usage: 1%                 IPv4 address for eth0: 172.31.27.213
  Swap usage:   0%

 * Ubuntu Pro delivers the most comprehensive open source security and
   compliance features.

   https://ubuntu.com/aws/pro

8 updates can be applied immediately.
To see these additional updates run: apt list --upgradable


Last login: Wed Feb  1 21:29:02 2023 from 128.62.16.255
(base) ubuntu@ip-172-31-27-213:~$ cd QuantumCircuitModelChecker/python_pkg/
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ cd ..
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker$ git submodule update --remote --merge
remote: Enumerating objects: 32, done.
remote: Counting objects: 100% (32/32), done.
remote: Compressing objects: 100% (6/6), done.
remote: Total 19 (delta 17), reused 15 (delta 13), pack-reused 0
Unpacking objects: 100% (19/19), 2.20 KiB | 90.00 KiB/s, done.
From https://github.com/trishullab/cflobdd
   3c55b4d..b56d092  main       -> origin/main
Updating 3c55b4d..b56d092
Fast-forward
 CFLOBDD/vector_complex_float_boost.cpp          |  5 +++
 CFLOBDD/vector_complex_float_boost.h            |  1 +
 CFLOBDD/vector_complex_float_boost_top_node.cpp | 18 ++++++++-
 CFLOBDD/vector_complex_float_boost_top_node.h   |  1 +
 cudd-complex-big/cplusplus/cuddObj.cc           |  6 +++
 cudd-complex-big/cplusplus/cuddObj.hh           |  1 +
 cudd-complex-big/cudd/cudd.h                    |  1 +
 cudd-complex-big/cudd/cuddUtil.c                | 49 +++++++++++++++++++++++--
 8 files changed, 77 insertions(+), 5 deletions(-)
Submodule path 'cflobdd': merged in 'b56d092a51d70d8db6fa5568f8f9c57c05ad59c5'
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker$ git pull
remote: Enumerating objects: 51, done.
remote: Counting objects: 100% (51/51), done.
remote: Compressing objects: 100% (13/13), done.
remote: Total 38 (delta 27), reused 35 (delta 25), pack-reused 0
Unpacking objects: 100% (38/38), 11.18 KiB | 159.00 KiB/s, done.
From https://github.com/cs14b052/QuantumCircuitModelChecker
   e28dc56..3f2c2fc  main       -> origin/main
Updating e28dc56..3f2c2fc
error: Your local changes to the following files would be overwritten by merge:
	Makefile
	python_pkg/grovers.py
	python_pkg/tasks.py
Please commit your changes or stash them before you merge.
Aborting
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker$ git stash
Saved working directory and index state WIP on main: e28dc56 Adding grover
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker$ git pull
Updating e28dc56..3f2c2fc
Fast-forward
 Makefile                                    |   6 +++---
 cflobdd                                     |   2 +-
 main.cpp                                    |  14 --------------
 python_pkg/QuantumModelCheckerTest.ipynb    |  50 --------------------------------------------------
 python_pkg/bv.py                            |   6 +++---
 python_pkg/dj.py                            |   4 ++--
 python_pkg/ghz.py                           |   4 ++--
 python_pkg/grovers.py                       |   4 ++--
 python_pkg/pyQuMC.py                        |  13 -------------
 python_pkg/qcmc_python_wrapper.cpp          |  86 --------------------------------------------------------------------------------------
 python_pkg/qft.py                           |   4 ++--
 python_pkg/quasimodo.py                     |  14 ++++++++++++++
 python_pkg/quasimodo_python_wrapper.cpp     |  89 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 python_pkg/simons.py                        |   4 ++--
 python_pkg/tasks.py                         |   8 ++++----
 python_pkg/test.py                          |  16 ----------------
 quantum_verifier.cpp => quantum_circuit.cpp | 169 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++---------------------------------------------------------------------------
 quantum_verifier.h => quantum_circuit.h     |  43 +++++++++++++++++++++++++------------------
 quantum_language_parser.cpp                 | 125 -----------------------------------------------------------------------------------------------------------------------------
 quantum_language_parser.h                   |  67 -------------------------------------------------------------------
 20 files changed, 243 insertions(+), 485 deletions(-)
 delete mode 100644 main.cpp
 delete mode 100644 python_pkg/QuantumModelCheckerTest.ipynb
 delete mode 100644 python_pkg/pyQuMC.py
 delete mode 100644 python_pkg/qcmc_python_wrapper.cpp
 create mode 100644 python_pkg/quasimodo.py
 create mode 100644 python_pkg/quasimodo_python_wrapper.cpp
 delete mode 100644 python_pkg/test.py
 rename quantum_verifier.cpp => quantum_circuit.cpp (90%)
 rename quantum_verifier.h => quantum_circuit.h (86%)
 delete mode 100644 quantum_language_parser.cpp
 delete mode 100644 quantum_language_parser.h
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker$ git stash pop
Auto-merging python_pkg/tasks.py
CONFLICT (content): Merge conflict in python_pkg/tasks.py
Auto-merging python_pkg/grovers.py
Auto-merging Makefile
CONFLICT (content): Merge conflict in Makefile
The stash entry is kept in case you need it again.
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker$ vi Makefile 
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker$ ls
LICENSE                          cudd_libcudd_la-pipefork.o  libcudd_la-cuddAddInv.o    libcudd_la-cuddCache.o    libcudd_la-cuddHarwell.o   libcudd_la-cuddSat.o       libcudd_la-cuddZddGroup.o  python_pkg
Makefile                         cudd_libcudd_la-prtime.o    libcudd_la-cuddAddIte.o    libcudd_la-cuddCheck.o    libcudd_la-cuddInit.o      libcudd_la-cuddSign.o      libcudd_la-cuddZddIsop.o   quantum_circuit.cpp
README.md                        cudd_libcudd_la-safe_mem.o  libcudd_la-cuddAddNeg.o    libcudd_la-cuddClip.o     libcudd_la-cuddInteract.o  libcudd_la-cuddSolve.o     libcudd_la-cuddZddLin.o    quantum_circuit.h
cflobdd                          cudd_libcudd_la-st.o        libcudd_la-cuddAddWalsh.o  libcudd_la-cuddCof.o      libcudd_la-cuddLCache.o    libcudd_la-cuddSplit.o     libcudd_la-cuddZddMisc.o   quantum_language_parser.o
cudd_libcudd_la-cpu_stats.o      cudd_libcudd_la-strsav.o    libcudd_la-cuddAndAbs.o    libcudd_la-cuddCompose.o  libcudd_la-cuddLevelQ.o    libcudd_la-cuddSubsetHB.o  libcudd_la-cuddZddPort.o   quantum_verifier.o
cudd_libcudd_la-cpu_time.o       cudd_libcudd_la-texpand.o   libcudd_la-cuddAnneal.o    libcudd_la-cuddDecomp.o   libcudd_la-cuddLinear.o    libcudd_la-cuddSubsetSP.o  libcudd_la-cuddZddReord.o
cudd_libcudd_la-cstringstream.o  cudd_libcudd_la-ucbqsort.o  libcudd_la-cuddApa.o       libcudd_la-cuddEssent.o   libcudd_la-cuddLiteral.o   libcudd_la-cuddSymmetry.o  libcudd_la-cuddZddSetop.o
cudd_libcudd_la-datalimit.o      libcudd_la-cuddAPI.o        libcudd_la-cuddApprox.o    libcudd_la-cuddExact.o    libcudd_la-cuddMatMult.o   libcudd_la-cuddTable.o     libcudd_la-cuddZddSymm.o
cudd_libcudd_la-epd.o            libcudd_la-cuddAbsVal.o     libcudd_la-cuddBddAbs.o    libcudd_la-cuddExport.o   libcudd_la-cuddPriority.o  libcudd_la-cuddUtil.o      libcudd_la-cuddZddUtil.o
cudd_libcudd_la-mtrBasic.o       libcudd_la-cuddAddAbs.o     libcudd_la-cuddBddCorr.o   libcudd_la-cuddGenCof.o   libcudd_la-cuddRead.o      libcudd_la-cuddWindow.o    libobj_la-cuddObj.o
cudd_libcudd_la-mtrGroup.o       libcudd_la-cuddAddApply.o   libcudd_la-cuddBddIte.o    libcudd_la-cuddGenetic.o  libcudd_la-cuddRef.o       libcudd_la-cuddZddCount.o  libqcmc.so
cudd_libcudd_la-pathsearch.o     libcudd_la-cuddAddFind.o    libcudd_la-cuddBridge.o    libcudd_la-cuddGroup.o    libcudd_la-cuddReorder.o   libcudd_la-cuddZddFuncs.o  main.o
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker$ cd python_pkg/
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ ls
__pycache__          bv_CFLOBDD_4.txt     dj_BDD_2048.txt      dj_WBDD_16.txt        ghz_CFLOBDD_2048.txt  grovers_BDD,_8.txt                     qft_BDD_32.txt       quasimodo_python_wrapper.cpp
bv.py                bv_CFLOBDD_4096.txt  dj_BDD_256.txt       dj_WBDD_32.txt        ghz_CFLOBDD_256.txt   grovers_BDD_16.txt                     qft_BDD_4.txt        results.txt
bv_BDD_1024.txt      bv_CFLOBDD_512.txt   dj_BDD_32.txt        dj_WBDD_4.txt         ghz_CFLOBDD_32.txt    grovers_BDD_4.txt                      qft_BDD_64.txt       run.sh
bv_BDD_128.txt       bv_CFLOBDD_64.txt    dj_BDD_4.txt         dj_WBDD_8.txt         ghz_CFLOBDD_4.txt     grovers_BDD_8.txt                      qft_BDD_8.txt        run2.sh
bv_BDD_16.txt        bv_CFLOBDD_8.txt     dj_BDD_4096.txt      ghz.py                ghz_CFLOBDD_4096.txt  grovers_CFLOBDD_16.txt                 qft_CFLOBDD_128.txt  run_final_computation.sh
bv_BDD_2048.txt      bv_WBDD_1024.txt     dj_BDD_512.txt       ghz_BDD_1024.txt      ghz_CFLOBDD_512.txt   grovers_CFLOBDD_4.txt                  qft_CFLOBDD_16.txt   simons.py
bv_BDD_256.txt       bv_WBDD_128.txt      dj_BDD_64.txt        ghz_BDD_128.txt       ghz_CFLOBDD_64.txt    grovers_CFLOBDD_8.txt                  qft_CFLOBDD_256.txt  simons_BDD_16.txt
bv_BDD_32.txt        bv_WBDD_16.txt       dj_BDD_8.txt         ghz_BDD_16.txt        ghz_CFLOBDD_8.txt     grovers_WBDD_16.txt                    qft_CFLOBDD_32.txt   simons_BDD_4.txt
bv_BDD_4.txt         bv_WBDD_256.txt      dj_CFLOBDD_1024.txt  ghz_BDD_2048.txt      ghz_WBDD_1024.txt     grovers_WBDD_4.txt                     qft_CFLOBDD_4.txt    simons_BDD_8.txt
bv_BDD_4096.txt      bv_WBDD_32.txt       dj_CFLOBDD_128.txt   ghz_BDD_256.txt       ghz_WBDD_128.txt      grovers_WBDD_8.txt                     qft_CFLOBDD_64.txt   simons_CFLOBDD_16.txt
bv_BDD_512.txt       bv_WBDD_4.txt        dj_CFLOBDD_16.txt    ghz_BDD_32.txt        ghz_WBDD_16.txt       libqcmc.so                             qft_CFLOBDD_8.txt    simons_CFLOBDD_4.txt
bv_BDD_64.txt        bv_WBDD_512.txt      dj_CFLOBDD_2048.txt  ghz_BDD_4.txt         ghz_WBDD_256.txt      log.txt                                qft_WBDD_128.txt     simons_CFLOBDD_8.txt
bv_BDD_8.txt         bv_WBDD_64.txt       dj_CFLOBDD_256.txt   ghz_BDD_4096.txt      ghz_WBDD_32.txt       out                                    qft_WBDD_16.txt      simons_WBDD_4.txt
bv_CFLOBDD_1024.txt  bv_WBDD_8.txt        dj_CFLOBDD_32.txt    ghz_BDD_512.txt       ghz_WBDD_4.txt        out.txt                                qft_WBDD_256.txt     simons_WBDD_8.txt
bv_CFLOBDD_128.txt   compute.py           dj_CFLOBDD_4.txt     ghz_BDD_64.txt        ghz_WBDD_512.txt      pyqcmc.cpython-39-x86_64-linux-gnu.so  qft_WBDD_32.txt      tasks.py
bv_CFLOBDD_16.txt    dj.py                dj_CFLOBDD_4096.txt  ghz_BDD_8.txt         ghz_WBDD_64.txt       qft.py                                 qft_WBDD_4.txt
bv_CFLOBDD_2048.txt  dj_BDD_1024.txt      dj_CFLOBDD_512.txt   ghz_CFLOBDD_1024.txt  ghz_WBDD_8.txt        qft_BDD_128.txt                        qft_WBDD_64.txt
bv_CFLOBDD_256.txt   dj_BDD_128.txt       dj_CFLOBDD_64.txt    ghz_CFLOBDD_128.txt   grovers.py            qft_BDD_16.txt                         qft_WBDD_8.txt
bv_CFLOBDD_32.txt    dj_BDD_16.txt        dj_CFLOBDD_8.txt     ghz_CFLOBDD_16.txt    grovers_BDD,_4.txt    qft_BDD_256.txt                        quasimodo.py
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ vi run.sh 
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ git add run.sh 
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ git commit -m "Updated run script"
U	Makefile
U	python_pkg/tasks.py
error: Committing is not possible because you have unmerged files.
hint: Fix them up in the work tree, and then use 'git add/rm <file>'
hint: as appropriate to mark resolution and make a commit.
fatal: Exiting because of an unresolved conflict.
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ git add ../Makefile 
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ vi run.sh 
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ git commit -m "Updated run script"
U	python_pkg/tasks.py
error: Committing is not possible because you have unmerged files.
hint: Fix them up in the work tree, and then use 'git add/rm <file>'
hint: as appropriate to mark resolution and make a commit.
fatal: Exiting because of an unresolved conflict.
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ vi tasks.py 
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ git add tasks.py 
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ git commit -m "Updated run script"
[main ce0b632] Updated run script
 Committer: Ubuntu <ubuntu@ip-172-31-27-213.ec2.internal>
Your name and email address were configured automatically based
on your username and hostname. Please check that they are accurate.
You can suppress this message by setting them explicitly. Run the
following command and follow the instructions in your editor to edit
your configuration file:

    git config --global --edit

After doing this, you may fix the identity used for this commit with:

    git commit --amend --reset-author

 4 files changed, 77 insertions(+), 9 deletions(-)
(base) ubuntu@ip-172-31-27-213:~/QuantumCircuitModelChecker/python_pkg$ vi run.sh 

#! /bin/bash

numRuns=50
range=12
options=('CFLOBDD' 'BDD' 'WBDD')

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=$range; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 ghz.py $numQubits $j $c >> ghz_${j}_${numQubits}.txt"
            timeout -v 15m python3 ghz.py $numQubits $j $c >> ghz_${j}_${numQubits}.txt
        done
    done
done

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=$range; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 bv.py $numQubits $j $c >> bv_${j}_${numQubits}.txt"
            timeout -v 15m python3 bv.py $numQubits $j $c >> bv_${j}_${numQubits}.txt
        done
    done
done

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=$range; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 dj.py $numQubits $j $c >> dj_${j}_${numQubits}.txt"
            timeout -v 15m python3 dj.py $numQubits $j $c >> dj_${j}_${numQubits}.txt
        done
    done
done

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=4; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 simons.py $numQubits $j $c >> simons_${j}_${numQubits}.txt"
            timeout -v 15m python3 simons.py $numQubits $j $c >> simons_${j}_${numQubits}.txt
        done
    done
done

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=8; i++))
    do
"run.sh" 83L, 2142B                                                                                                                                                                                         1,1           Top
