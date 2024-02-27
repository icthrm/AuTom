#include "opt.h"

void usageDual() {

	printf("Usage: recon_single [options] [input_file] [output_file]\n");

	printf("--gpu/-G number\n");
	printf("  Run the reconstruction using the number of GPUs\n");

	printf("--mpi/-M number\n");
	printf(
			"  Run the reconstruction with the number of threads on the cluster\n");

	printf("--mode/-m FBP/-F filter name\n");
	printf("  Name of the filter for the reconstructed method FBP\n");

	printf("--mode/-m SIRT Iteration numbers, relax parameter\n");
	printf(
			"  Iteration numbers and relax parameter for the reconstructed method SIRT\n");

	printf("--mode/-m ASART Itertaion numbers, relax parameter\n");
	printf(
			"  Iteration numbers and relax parameter for the reconstructed method ASART\n");

	printf("--tiltangle/-t File name\n");
	printf("  Alignment for tiltangle\n");

	printf("--xtiltangle/-x File name\n");
	printf("  Alignment for xtiltangle\n");

	printf("--geometry/-g offset,zshift,thickness\n");
	printf("  four parameters for geometry\n");

	printf("--input/-i File name");
	printf("  Input image file with aligned projections.\n");
	printf("--output/-o File name");
	printf("  Output file for reconstruction or reprojection.\n");

	printf("--help/-h");
	printf("   Help Information\n");

}

void usageSig() {

	printf("Usage: recon_single [options] [input_file] [output_file]\n");

	printf("--gpu/-G number\n");
	printf("  Run the reconstruction using the number of GPUs\n");

	printf("--mpi/-M number\n");
	printf(
			"  Run the reconstruction with the number of threads on the cluster\n");

	printf("--mode/-m FBP/-F filter name\n");
	printf("  Name of the filter for the reconstructed method FBP\n");

	printf("--mode/-m SIRT Iteration numbers, relax parameter\n");
	printf(
			"  Iteration numbers and relax parameter for the reconstructed method SIRT\n");

	printf("--mode/-m ASART Itertaion numbers, relax parameter\n");
	printf(
			"  Iteration numbers and relax parameter for the reconstructed method ASART\n");

	printf("--tiltangle/-t File name\n");
	printf("  Alignment for tiltangle\n");

	printf("--geometry/-g offset,xaxistilt,zshift,thickness\n");
	printf("  four parameters for geometry\n");

	printf("--input/-i File name");
	printf("  Input image file with aligned projections.\n");
	printf("--output/-o File name");
	printf("  Output file for reconstruction or reprojection.\n");

	printf("--help/-h");
	printf("   Help Information\n");

}

void printOpt(options* opt) {
	cout << "GPU nr_process=" << opt->gpu_process << endl;
	cout << "MPI nr_process=" << opt->mpi_process << endl;
	cout << "tiltangle=" << opt->tiltangle << endl;
	cout << "xtiltangle=" << opt->xtiltangle << endl;
	cout << "xaxistilt= " << opt->xaxistilt << endl;
	cout << "zshift=" << opt->zshift << endl;
	cout << "thickness=" << opt->thickness << endl;
	cout << "offset=" << opt->offset << endl;
	cout << "input=" << opt->input << endl;
	cout << "output=" << opt->output << endl;
	cout << "initial=" << opt->initial << endl;
	cout << "filter=" << opt->filter << endl;
	cout << "method=" << opt->method << endl;
	cout << "iter=" << opt->iter_num << endl;
	cout << "step=" << opt->step << endl;
}
void initOpt(options* opt) {
	opt->gpu_process = 0;
	opt->mpi_process = 0;
	opt->tiltangle = "tilt";
	opt->xtiltangle = "offset";
	opt->xaxistilt = 0;
	opt->zshift = 0;
	opt->thickness = 0;
	opt->offset = 0;
	opt->input = "input";
	opt->output = "output";
	opt->initial = "initial";
}
int GetOptsDual(int argc, char **argv, options* opts_) {

	static struct option longopts[] = { { "help", no_argument, NULL, 'h' }, //1
			{ "gpu", required_argument, NULL, 'G' }, //2
			{ "mpi", required_argument, NULL, 'M' }, //3
			{ "tiltangle", required_argument, NULL, 't' }, //7
			{ "xtiltangle", required_argument, NULL, 'x' }, //8
			{ "input", required_argument, NULL, 'i' }, //10
			{ "output", required_argument, NULL, 'o' }, //11
			{ "initial", required_argument, NULL, 'n' }, //12
			{ "geometry", required_argument, NULL, 'g' }, //9
			{ "mode", required_argument, NULL, 'm' }, { NULL, 0, NULL, 0 } };
	//if((argc != 18 && argc != 23) && argc >= 3 || (argc == 2 && argv[1][0] != '-' && argv[1][1] != 'h') || argc == 1) {

	if ((argc == 2 && argv[1][0] != '-' && argv[1][1] != 'h') || argc == 1) {
		/*	EX_TRACE(
		 "[--gpu/-G number]\n"
		 "[--mpi/-M number]\n"
		 "[--tiltangle/-t File name ]\n"
		 "[--xtiltangle/-x File name]\n"
		 "[--geometry/-g xaxistilt,zshift,thickness]\n"
		 "[--input/-i File name]\n"
		 "[--output/-o File name]\n"
		 "[--initial/-n File name]\n"
		 "Iteration Method: [--mode/-m method(SIRT or ASART) Iteration numbers,relax parameter]\n"
		 "FBP Method: [--mode/-m method(FBP) filter name]\n");*/
		usageDual();
		return -1;
	}

	int ch;

	string tmp;
	while ((ch = getopt_long(argc, argv, "hG:M:t:x:g:i:o:n:m:", longopts, NULL))
			!= -1) {
		switch (ch) {

		case '?':
			EX_TRACE("Invalid option '%s'.", argv[optind - 1])
			;
			return -1;

		case ':':
			EX_TRACE("Missing option argument for '%s'.", argv[optind - 1])
			;
			return -1;

		case 'h':
			/*EX_TRACE(
			 "[--gpu/-G number]\n"
			 "[--mpi/-M number]\n"
			 "[--tiltangle/-t File name ]\n"
			 "[--xtiltangle/-x File name]\n"
			 "[--geometry/-g offset,xaxistilt,zshift,thickness]\n"
			 "[--input/-i File name]\n"
			 "[--output/-o File name]\n"
			 "[--initial/-n File name]\n"
			 "Iteration Method: [--mode/-m method(SIRT or ASART) Iteration numbers,relax parameter]\n"
			 "FBP Method: [--mode/-m method(FBP) filter name]\n");*/
			usageDual();
			return 0;

		case 'G': {
			stringstream iss(optarg);
			iss >> opts_->gpu_process;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.\n", optarg);
				return -1;
			}

		}
			break;

		case 'M': {
			stringstream iss(optarg);
			iss >> opts_->mpi_process;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}
		}
			break;
		case 't': {
			stringstream iss(optarg);
			iss >> opts_->tiltangle;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}
		}
			break;

		case 'x': {
			stringstream iss(optarg);
			iss >> opts_->xtiltangle;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}
		}
			break;

		case 'i': {
			stringstream iss(optarg);
			iss >> opts_->input;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}
		}
			break;

		case 'o': {
			stringstream iss(optarg);
			iss >> opts_->output;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
			}
		}
			break;

		case 'n': {
			stringstream iss(optarg);
			iss >> opts_->initial;

			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
			}
		}
			break;

		case 'g': //offset,xaxistilt,zshift,thickness
		{
			stringstream iss(optarg);
			getline(iss, tmp, ',');
			opts_->offset = atof(tmp.c_str());

			getline(iss, tmp, ',');
			opts_->xaxistilt = atof(tmp.c_str());

			getline(iss, tmp, ',');
			opts_->zshift = atof(tmp.c_str());

			getline(iss, tmp);
			opts_->thickness = atoi(tmp.c_str());

			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}

		}
			break;

		case 'm': {
			std::istringstream iss(optarg);

			if ( strcmp(optarg,"BPT") ) {

				getline(iss, opts_->method, ',');

				if (opts_->method == "FBP") {
					getline(iss, opts_->filter);
				} else if (opts_->method == "ASIRT" || opts_->method == "SIRT"
						|| opts_->method == "SART"
						|| opts_->method == "ASART") {
					getline(iss, tmp, ',');
					opts_->iter_num = atoi(tmp.c_str());
					getline(iss, tmp);
					opts_->step = atof(tmp.c_str());
				} else {
					EX_TRACE(
							"Iteration Method: [--mode/-m method(SIRT or ASART) Iteration numbers,relax parameter]\n"
									"FBP Method: [--mode/-m method(FBP) filter name]\n");
				}

			}
			else{
				getline(iss, opts_->method);
			}
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}
		}
			break;

		case 0:
			break;

		default:
			assert(false);
		} //end switch
	} //end while
	return 1;
}

int GetOptsSig(int argc, char **argv, options* opts_) {

	static struct option longopts[] = { { "help", no_argument, NULL, 'h' }, //1
			{ "gpu", required_argument, NULL, 'G' }, //2
			{ "mpi", required_argument, NULL, 'M' }, //3
			{ "tiltangle", required_argument, NULL, 't' }, //7
			{ "input", required_argument, NULL, 'i' }, //10
			{ "output", required_argument, NULL, 'o' }, //11
			{ "initial", required_argument, NULL, 'n' }, //12
			{ "geometry", required_argument, NULL, 'g' }, //9
			{ "mode", required_argument, NULL, 'm' }, { NULL, 0, NULL, 0 } };
	//if((argc != 18 && argc != 23) && argc >= 3 || (argc == 2 && argv[1][0] != '-' && argv[1][1] != 'h') || argc == 1) {

	if ((argc == 2 && argv[1][0] != '-' && argv[1][1] != 'h') || argc == 1) {
		usageSig();
		//return -1;
		exit(0);
	}

	int ch;

	string tmp;
	while ((ch = getopt_long(argc, argv, "hG:M:t:g:i:o:n:m:", longopts, NULL))
			!= -1) {
		switch (ch) {

		case '?':
			EX_TRACE("Invalid option '%s'.", argv[optind - 1])
			;
			return -1;

		case ':':
			EX_TRACE("Missing option argument for '%s'.", argv[optind - 1])
			;
			return -1;

		case 'h':
			/*EX_TRACE(
			 "[--gpu/-G number]\n"
			 "[--mpi/-M number]\n"
			 "[--tiltangle/-t File name ]\n"
			 "[--xtiltangle/-x File name]\n"
			 "[--geometry/-g offset,xaxistilt,zshift,thickness]\n"
			 "[--input/-i File name]\n"
			 "[--output/-o File name]\n"
			 "[--initial/-n File name]\n"
			 "Iteration Method: [--mode/-m method(SIRT or ASART) Iteration numbers,relax parameter]\n"
			 "FBP Method: [--mode/-m method(FBP) filter name]\n")
			 ;*/
			usageSig();
			//return 0;
			exit(0);

		case 'G': {
			stringstream iss(optarg);
			iss >> opts_->gpu_process;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.\n", optarg);
				return -1;
			}

		}
			break;

		case 'M': {
			stringstream iss(optarg);
			iss >> opts_->mpi_process;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}
		}
			break;
		case 't': {
			stringstream iss(optarg);
			iss >> opts_->tiltangle;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}
		}
			break;

		case 'i': {
			stringstream iss(optarg);
			iss >> opts_->input;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}
		}
			break;

		case 'o': {
			stringstream iss(optarg);
			iss >> opts_->output;
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
			}
		}
			break;

		case 'n': {
			stringstream iss(optarg);
			iss >> opts_->initial;

			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
			}
		}
			break;

		case 'g': //offset,xaxistilt,zshift,thickness
		{
			stringstream iss(optarg);
			getline(iss, tmp, ',');
			opts_->offset = atof(tmp.c_str());

			getline(iss, tmp, ',');
			opts_->xaxistilt = atof(tmp.c_str());

			getline(iss, tmp, ',');
			opts_->zshift = atof(tmp.c_str());

			getline(iss, tmp);
			opts_->thickness = atoi(tmp.c_str());

			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}

		}
			break;

		case 'm': {
			std::istringstream iss(optarg);

			if (strcmp(optarg, "BPT")) {
				getline(iss, opts_->method, ',');

				if (opts_->method == "FBP") {
					getline(iss, opts_->filter);
				} else if (opts_->method == "ASIRT" || opts_->method == "SIRT"
						|| opts_->method == "SART"
						|| opts_->method == "ASART") {
					getline(iss, tmp, ',');
					opts_->iter_num = atoi(tmp.c_str());
					getline(iss, tmp);
					opts_->step = atof(tmp.c_str());
				} else {
					EX_TRACE(
							"ERROR METHOD!!!\n"
									"Iteration Method: [--mode/-m method(SIRT or ASART) Iteration numbers,relax parameter]\n"
									"FBP Method: [--mode/-m method(FBP) filter name]\n");
				}
			} else {
				getline(iss, opts_->method);
			}
			if (iss.fail()) {
				EX_TRACE("Invalid argument '%s'.", optarg);
				return -1;
			}
		}
			break;

		case 0:
			break;

		default:
			assert(false);
		} //end switch
	} //end while
	return 1;
}

