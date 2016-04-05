import glob, os, pipes, subprocess

setname = "overpass_stopsign_set5"
directory = "../StreetviewScraper/Scraped/"
inputdirectory = directory + setname + "/"
#outputdirectory = directory + setname + "_output/"
outputdirectory = directory + setname + "_output/"

if not os.path.exists(outputdirectory):
	os.makedirs(outputdirectory)

os.chdir(inputdirectory)
for file in glob.glob("*.png"):
	print(file)
	#args = "./findstopsigns {} {}".format(pipes.quote(file), pipes.quote("warped_" + file))
	subprocess.call(["findstopsigns", file, outputdirectory + "warped_" + file])
