Instructions
------------

In order to generate experimental booklets from materials, the following
process should be followed. (a more detailed step-by-step methodology of the
utilities function is described in method.txt)

1) write the experimental and filler materials and 
	
	example files are to be found in the example?/materials directories -
	they should take the form:

		<tag> <material 1 version 1>
		<tag> <material 1 version 2>
		<tag> <material 1 version 3>
		<tag> <material 1 version 4>

		<tag> <material 2 version 1>
		<tag> <material 2 version 2>
		etc

	It is important that:
		a) there is no leading blank line before the first material
		b) there are no trailing spaces on each line.
		c) there is a single, blank, line between each set of versions.
		d) there IS a single, blank, line at the end of the file.
		e) the tags contain no spaces or tabs.
		f) the filenames of experimental materials are alphanumerically
		   named such that they appear before the filler matierals.
		   (e.g., a_experiment1, z_filler1, z_filler2)

2) create an experiment directory
	i)	make a new experiemental directory (e.g. prodprim), with a subdirectory
		called materials.
	ii)	copy the n.files (0.spec, 1.check_me, etc) into the new directory
		from the template or an appropriate example directory - use example2
		if you are making booklets or example1 if you are simply randomizing
		materials for further processing. try the examples out to see if
		one closely matches your requirements.

	anyway, you should have a directory structure that looks something like:

prodprim/
	0.spec
	1.check_me
	2.latin_me
	3.random_me
	4.output_me
	5.dot_me
	6.tar_me
	7.tidy_me
	materials/
		a_materials
		filler1
		filler2
		filler3
		filler4

3) modify the 0.spec file to match your experimental materials.
	0.spec defines your experimental materials, and what you wish to do.
	you should be very careful changing the preamble setting -
	it affects a lot what the final (or dotted) output will look like.

	here is an example: 

		#! /bin/sh

		sets=32			# this is how many MATERIAL sets are in each file.
		items=4			# this is how many item VERSIONS of each MATERIAL you have (items)
		subjects=80		# this is how many booklets are required (how many subjects you are running over)

		#	the following 3 are only used in the 'dot_me' stage
		preamble=-
		splitstr=". "
		splitpoint=-1	# the minus sign is important - strip the tag out.
	
	alter "sets", "items", and "subjects" to match your number of
	materials, number of versions of each material, and the number
	of subjects you are intending to create for.

4) run the scripts in ascending numerical order, to perform each phase:
	(make sure the shell scripts are executable - you may also need to
	invoke them using ./1.check_me, etc, depending on your PATH)

	i) 1.check_me - file validity checking
		this tests adherence to the restrictions, outlined above,
		of the materials.
		if this phase generates any output other than
			[materials/a_material] OK
		action *must* be taken to rectify the errors before continuing.
		errors may include:
			[materials/a_material] (set 4, item 1) blank line.
				one of your materials is missing a version.

	ii)	2.latin_me
		this creates a latin/ directory, which contains latin squared
		versions of your materials - you can edit each of the files in the
		latin directory to see what has happened, but basically you have N
		latin squared subsets of your original materials.

		NB this version does not cater for experiments where latin squaring
		is not required. please contact the author if a non-latin-squared
		split program is required.

	iii) 3.random_me
		this takes the output of the latin stage, and generates a
		randomised version of each latin squared variant of the group of
		material files. this process is repeated "subject" times, so in
		fact you end up with "subject"*"versions" randomised files. they
		appear in the random/ directory

	iv)	4.output_me
		this takes each of the groups of experimental files, and combines
		them to create the final, randomised, experiment files - it is very
		careful to space the experimental matierals evenly through the
		output file - this release only supports this careful insertion for
		the very first file in the materials subdirectory - this is why it
		is important that this is the first file...

		the resulting combined files are placed in the output/ directory.
		they look like files named:
			000-A	- contains material 1 version 1, mat 2 ver 2, etc
			000-B	- contains material 1 version 2, mat 2 ver 3, etc
			000-C	- contains material 1 version 3, mat 2 ver 4, etc
			000-D	- contains material 1 version 4, mat 2 ver 1, etc

	v) 5.dot_me (optional)
		for creating booklets, this optional stage will take the output/
		files and create experimental booklets (for sentence completion
		experiments).

		*** you should print the booklets now ***

	vi)	6.tar_me (optional)
		for creating N compressed, tape archives of the  -A, -B, etc sets.

5) tidy up.
	the intermediate steps, above, use quite a bit of disk space, so you
	may wish to run 7.tidy_me to clear out everything except the
	experimental materials, between runs. note that this will
	destroy the current set of booklet files... so make sure they
	have printed correctly before running this script!
		
---
Psycho version 4.50, 29th July 2004

