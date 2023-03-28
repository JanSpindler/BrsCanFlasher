import os


if __name__ == '__main__':
	current_dir = os.path.abspath(__file__)
	print(current_dir)
	os.system(f'cmake -DBRS_PROJ_PATH={current_dir} .')
