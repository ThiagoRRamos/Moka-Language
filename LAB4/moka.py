#!/usr/bin/python

import os
import subprocess
import sys

def generate_file(basename, flexname=None, yaccname=None, outname=None, dataname=None, resultname=None):
	if not flexname:
		flexname = basename + '.l'
	if not yaccname:
		yaccname = basename + '.y'
	if not outname:
		outname = basename + '.out'
	if not resultname:
		resultname = basename+'.res'
	if not dataname:
		dataname = basename+'.dat'
	if subprocess.call(['flex', flexname]):
		print "Problema com o flex"
		return;
	if subprocess.call(['yacc', yaccname]):
		print "Problema com o yacc"
		return;
	if subprocess.call(['gcc', 'y.tab.c', 'main.c', 'yyerror.c', '-o', outname, '-lfl']):
		print "Problema com o gcc"
		return;
	subprocess.call(['touch', resultname])
	#if subprocess.call(['./' + outname], stdin=open(dataname)):
	if subprocess.call(['./' + outname], stdin=open(dataname), stdout=open(resultname, 'w')):
		print "Falha na execucao"


if len(sys.argv) > 1:
	generate_file(*sys.argv[1:])
else:
	print "Quantidade de termos insuficiente"