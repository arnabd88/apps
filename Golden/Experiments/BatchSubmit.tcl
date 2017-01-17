## This is a top level script batch regression :
## 1. Generating slurm scripts for each run
## 2. Submitting the batch jobs
## 3. Tracking the submitted jobs to completion

proc FileToList { source_file } {
	set orgfile [open $source_file {RDONLY}]
	set contents [split [read -nonewline $orgfile] \n]
	list $contents
	close $orgfile
	return $contents

}

proc ListToFile { name templist path file_type } {
	exec /bin/rm -rf $path/$name.$file_type
	set infile [open $path/$name.$file_type {RDWR CREAT APPEND}]
	set maxlength [llength $templist]
	for {set i 0} { $i < $maxlength } { incr i } {
		set newline [lindex $templist $i]
		puts $infile $newline
	}
	close $infile
}

proc Info { str1 str2 } {
	puts "INFO:$str1 --- $str2 --- "
}

proc getRunDir { } {
	if { [lsearch $::argv "-p"] == -1 } {
		puts "INFO: ERROR ---- The Run directory is unspecified ----"
		exit
	} else {
		set RunDir [lindex $::argv [expr {[lsearch $::argv "-p"] + 1}]]
		if {[file isdirectory $RunDir]} {
			return $RunDir
		} else {
			puts "Info: ERROR ---- The Run directory does not exists ----"
			exit
		}
	}
}

proc getBenchList { RunDir } {
	set finalList ""
	if { [lsearch $::argv "-fl"] == -1 } {
		puts "INFO: ERROR ---- Benchmark List not provided ----"
		exit
	} else {
		set fname [lindex $::argv [expr {[lsearch $::argv "-fl"] + 1}]]
		set blist [FileToList $fname]
		puts $blist
		for {set i 0} { $i < [llength $blist] } {incr i } {
			puts $RunDir/[lindex $blist $i]
			if {[file isdirectory $RunDir/[lindex $blist $i]]} {
				lappend finalList [lindex $blist $i]
			}
		}
		return $finalList

	}
}

proc dumpMake { exe bench cat } {
	set infile [open common_$bench.mk w]
	puts $infile "CAT_NAME=\"$cat\""
	puts $infile "EX_NAME=\"$bench\""
	puts $infile "FCP=\"num\""
	puts $infile "TARGETS=ex_$exe\_gep"
	puts $infile "include ../../../common.mk"
	close $infile
}

proc dumpAndSubmitBatchScript { exe bench cat } {
	set infile [open batch.slurm w]
	puts $infile "\#\!\/bin\/csh"
	puts $infile "#SBATCH --time=12:00:00"
	puts $infile "#SBATCH --partition=soc-kp"
	puts $infile "#SBATCH --account=soc-kp"
	puts $infile "#SBATCH -N 1"
	puts $infile "#SBATCH -n 16"
	puts $infile "#SBATCH -J $bench\_log"
	puts $infile "#SBATCH -o $bench\_out"
	puts $infile "#SBATCH -o $bench\_err"
	puts $infile "#SBATCH --mail-type=ALL"
	puts $infile "#SBATCH --mail-user=arnab.d.88@gmail.com"
	puts $infile "set CC=\/uufs\/chpc.utah.edu\/sys\/installdir\/gcc\/5.4.0\/bin\/gcc"
	puts $infile "set CXX=\/uufs\/chpc.utah.edu\/sys\/installdir\/gcc\/5.4.0\/bin\/g++"
	puts $infile "make -f common_$bench.mk"
	close $infile
	catch {exec sbatch batch.slurm} msg
	puts $msg
}

proc generateMake { ori dti bench cat} {
	set cdir [pwd]
	cd $ori
	dumpMake $ori $bench $cat
	cd $cdir
	cd $dti
	dumpMake $dti $bench $cat
	cd $cdir
}

proc generateBatch { ori dti bench cat } {
	set cdir [pwd]
	cd $ori
	dumpAndSubmitBatchScript $ori $bench $cat
	cd $cdir
	cd $dti
	dumpAndSubmitBatchScript $dti $bench $cat
	cd $cdir
}

proc SetUpAndSubmit { RunDir benchList Cat} {
	set blen [llength $benchList]
	if {$blen <= 0} {
		puts "INFO: ERROR ---- No BenchMark selected for simulations ----"
		exit
	}
	set homeDir [pwd]
	cd $RunDir
	for {set i 0} { $i < $blen } { incr i } {
		set bench [lindex $benchList $i]
		set benchDir "$RunDir/$bench/"
		puts $bench
		cd $benchDir
		if {[file exists "ori"]} {
			exec rm -rf ori
		} 
		if {[file exists "dti"]} {
			exec rm -rf dti
		}
		exec mkdir ori dti
		generateMake "ori" "dti" $bench $Cat
		generateBatch "ori" "dti" $bench $Cat
	}
	cd $homeDir
	
}


proc getCatName { } {
	if {[lsearch $::argv "-cat"] == -1} {
		Info "ERROR" "Category name unspecified"
		exit
	} else {
		return [lindex $::argv [expr {[lsearch $::argv "-cat"] + 1}]]
	}
}

proc main { } {
	set homedir [pwd]
	set RunDir [ getRunDir ]
	set Cat [ getCatName ]
	set benchList [ getBenchList $RunDir]
	SetUpAndSubmit $RunDir $benchList $Cat
	#GenBatchScript $RunDir $benchList
}

main

