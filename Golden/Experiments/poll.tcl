

set len 1000
while {$len > 1} {
	after 1800000
	catch {exec squeue -u u1014840} msg
	set jlist [split $msg "\n"]
	set len [llength $jlist]
	puts "INFO: ---- Checking After 30 mins ----"
	puts "Remaining Jobs: [expr {$len - 1}]"
}
