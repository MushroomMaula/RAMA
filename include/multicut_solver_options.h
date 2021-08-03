#pragma once
#include <CLI/CLI.hpp>

struct multicut_solver_options {
    std::string input_file;
    std::string output_sol_file="";
    int max_cycle_length_lb = 5;
    int num_dual_itr_lb = 10;
    int max_cycle_length_gaec = 3;
    int num_dual_itr_gaec = 5;
    float mean_multiplier_mm = 0.05;
    float max_multiplier_contraction = 0.4;
    float matching_thresh_crossover_ratio = 0.005; 
    float tri_memory_factor = 2.0;
    bool only_compute_lb = false;
    int max_time_sec = -1;

    multicut_solver_options() {}
    multicut_solver_options(
        const int _max_cycle_length_lb, 
        const int _num_dual_itr_lb, 
        const int _max_cycle_length_gaec, 
        const int _num_dual_itr_gaec, 
        const float _mean_multiplier_mm,
        const float _max_multiplier_contraction,
        const float _matching_thresh_crossover_ratio,
        const float _tri_memory_factor,
        const bool _only_compute_lb,
        const int _max_time_sec) :
        max_cycle_length_lb(_max_cycle_length_lb), 
        num_dual_itr_lb(_num_dual_itr_lb), 
        max_cycle_length_gaec(_max_cycle_length_gaec), 
        num_dual_itr_gaec(_num_dual_itr_gaec), 
        mean_multiplier_mm(_mean_multiplier_mm),
        max_multiplier_contraction(_max_multiplier_contraction),
        matching_thresh_crossover_ratio(_matching_thresh_crossover_ratio),
        tri_memory_factor(_tri_memory_factor),
        only_compute_lb(_only_compute_lb),
        max_time_sec(_max_time_sec)
    {}

    int from_cl(int argc, char** argv) {
        CLI::App app{"Solver for multicut problem. "};
        app.add_option("-f,--file,file_pos", input_file, "Path to multicut instance (.txt)")->required()->check(CLI::ExistingPath);
        app.add_option("-o,--out_sol_file", output_sol_file, "Path to save node labeling (.txt)");
        app.add_option("max_cycle_dual", max_cycle_length_lb, "Maximum length of conflicted cycles to consider for initial dual updates. (Default: 5).")->check(CLI::Range(0, 5));
        app.add_option("dual_itr", num_dual_itr_lb, "Number of dual update iterations per cycle. (Default: 10).")->check(CLI::NonNegativeNumber);
        app.add_option("max_cycle_primal", max_cycle_length_gaec, "Maximum length of conflicted cycles to consider during primal iterations for reparameterization. (Default: 3).")->check(CLI::Range(0, 5));
        app.add_option("dual_itr_primal", num_dual_itr_gaec, "Number of dual update iterations per cycle during primal reparametrization. (Default: 5).")->check(CLI::NonNegativeNumber);
        app.add_option("mean_multiplier_mm", mean_multiplier_mm, "Match the edges which have cost more than mean(pos edges) * mean_multiplier_mm.")->check(CLI::NonNegativeNumber);
        app.add_option("max_multiplier_contraction", max_multiplier_contraction, "Contract the edges which have cost more than max(pos edges) * max_multiplier_contraction.")->check(CLI::Range(0.0f, 1.0f));
        app.add_option("matching_thresh_crossover_ratio", matching_thresh_crossover_ratio, "Ratio of (# contract edges / # nodes ) at which to change from maximum matching based contraction to threshold based. "
            "(Default: 0.1). Greater than 1 will always use thresholding.")->check(CLI::NonNegativeNumber);
        app.add_option("tri_memory_factor", tri_memory_factor, 
            "Average number of triangles per repulsive edge. (Used for memory allocation. Use lesser value in-case of out of memory errors during dual solve). (Default: 2.0).")->check(CLI::PositiveNumber);
        app.add_flag("--only_lb", only_compute_lb, "Only compute the lower bound. (Default: false).");
        try {
            app.parse(argc, argv);
            return -1;
        } catch (const CLI::ParseError &e) {
            return app.exit(e);
        }
    }
};