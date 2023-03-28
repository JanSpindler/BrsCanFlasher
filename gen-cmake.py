import os


if __name__ == '__main__':
	current_dir = 'C:/Users/jspin/CodeProjects/Test/BrsCanFlasher/'
	os.system(f'cmake -DBRS_PROJ_PATH={current_dir} .')
