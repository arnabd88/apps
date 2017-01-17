import sys
import re
import copy
import math
import numpy
import os


pIndex = -1
fIndex = -1
path = ''

def disp( typ, stat ):
	print "INFO: ",typ," => ",stat,"\n"

if('-p' in sys.argv):
	pIndex = sys.argv.index('-p')
	path = sys.argv[pIndex + 1]
else:
	disp("ERROR", "Results Path Not Provided, Exiting!!")
	sys.exit()


def parseargfile(fname):
	fdict = {}
	tempList = []
	start = 0
	catName = ""
	contents = open(fname, 'r+').read().splitlines()
	for lines in contents:
		if ("cat" in lines.split(":") and len(lines.split(":"))>1):
			if(start==1):
				fdict[catName] = tempList
				start = 0
			catName = lines.split(":")[1]
			tempList = []
		else:
			if(start==0):
				start=1
			tempList.append(lines)
	if(start==1):
		fdict[catName] = tempList
	return fdict
		

def CheckDirExists(path, fdict):
	finalDict = {}
	tempList = []
	for key,sublist in fdict.iteritems():
		for i in sublist:
			oriPath = path+"/"+i+"/ori/"
			dtiPath = path+"/"+i+"/dti/"
			oricsv  = oriPath+"/result_"+i+"_ori_gep.csv"
			dticsv  = dtiPath+"/result_"+i+"_dti_gep.csv"
			if(os.path.isdir(oriPath) and os.path.isdir(dtiPath)):
				if(os.path.isfile(oricsv) and os.path.isfile(dticsv)):
					tempList.append(i)
				else:
					disp("Warning", "CSV file do not exists for "+i)
			else:
				disp("Warning", "Simulation directories(ori/dti) do not exists for "+i)
		if(len(tempList)>0):
			finalDict[key] = tempList
		else:
			disp("Warning", "Aborting collection for item "+i)
		tempList = []
	return finalDict

def CheckConsistency(path, fname):
	if(os.path.isfile(fname)):
		# fdict = parseargfile(fname)
		# finalDict = CheckDirExists(path,fdict)
		# return finalDict
		return CheckDirExists(path,parseargfile(fname))
	else:
		disp("ERROR", "File does not exists... Exiting!!")
		sys.exit()
	

def parsearg():
	if('-p' in sys.argv):
		pIndex = sys.argv.index('-p')
		path = sys.argv[pIndex + 1]
	else:
		disp("ERROR", "Results Path Not Provided, Exiting!!")
		sys.exit()
	
	if('-fl' in sys.argv):
		fIndex = sys.argv.index('-fl')
		fname = sys.argv[fIndex + 1]
	else:
		disp("ERROR", "FileList not Provided, Exiting!!")
		sys.exit()

	if('-o' in sys.argv):
		oIndex = sys.argv.index('-o')
		oname = sys.argv[oIndex + 1]
	else:
		disp("OutFile not given", "Defaulting to Results.txt")
		oname = "Results.txt"

	fdict = CheckConsistency(path, fname)
	if(len(fdict.keys()) == 0):
		disp("ERROR", "FileList is empty... Exiting !!")
		sys.exit()
	return [path, fdict, fname, oname]

def parse_csv_res(path, name, od):
	datalist = [0,0,0,0,0,0] # ['exec', 'sdc', 'benign', 'crashes', 'sdc-detect', 'benign-detect'
	if(od==0): ## for ori
		contents = open(path+"/"+name+"/ori/result_"+name+"_ori_gep.csv", 'r+').read().splitlines()
	else:
		contents = open(path+"/"+name+"/dti/result_"+name+"_dti_gep.csv", 'r+').read().splitlines()
	datalist[0] = len(contents[1:])
	for i in contents[1:]:
		ilist = i.split(",")
		##---SDC---
		if(int(ilist[5]) != 0):
			datalist[1] = datalist[1]+1
			if(int(ilist[11]) != 0):
				datalist[4] = datalist[4]+1
		##---BENIGN---
		if(int(ilist[6]) != 0):
			datalist[2] = datalist[2]+1
			if(int(ilist[11]) != 0):
				datalist[5] = datalist[5]+1

		##--- Crashes ---
		if(int(ilist[7]) != 0):
			datalist[3] = datalist[3]+1

	return datalist





		

def populateRes(path, fdict):
	resDict = {}
	tempList = []
	for key, sublist in fdict.iteritems():
		tempList = []
		for i in sublist:
			sub_ori_info = parse_csv_res(path, i, 0) ## 0: for ori
			sub_dti_info = parse_csv_res(path, i, 1) ## 1: for dti
			tempList.append(sub_ori_info+sub_dti_info)
			#tempList.append(sub_dti_info)
		resDict[key] = tempList
	return resDict



def writeResults(oname, fdict, resultDict):
	fres = open(os.getcwd()+"/"+oname, 'w')
	print os.getcwd()+oname
	for key, value in fdict.iteritems():
		for i in range(0,len(resultDict[key])):
			stat = resultDict[key][i]
			name = value[i]
			fres.write('%15s' % name)
			for j in stat:
				fres.write('%10s' % j)
			fres.write("\n")


def main():
	[path, fdict, fname, oname] = parsearg()
	resultDict = populateRes(path, fdict)
	writeResults(oname,fdict, resultDict)




main()
	
