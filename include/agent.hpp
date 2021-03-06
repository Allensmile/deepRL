#ifndef AGENT_HPP_
#define AGENT_HPP_

#include <random>
#include "memoryreplay.hpp"

namespace deepRL {
/**
 * Deep Q-Network
 */
class DeepQLearner {
public:
  DeepQLearner(
    const ActionVect& legal_actions,
    double epsilon_start,
    const double epsilon_min,
    const double epsilon_decay,
    const int epsilon_explore_idx,
    const int replay_batch_size,
    const int replay_memory_capacity,
    const int replay_start_size,
    const std::string sampleStrategy,
    const int update_frequency,
    const double discount_factor,
    const std::string solver_param,
    const bool evaluate_,
    const double eval_epsilon_,
    const int target_q_freq_,
    const bool doubleQ_
    ):
      legal_actions_(legal_actions),
      epsilon_start_(epsilon_start),
      epsilon_(epsilon_start),
      epsilon_min_(epsilon_min),
      epsilon_decay_(epsilon_decay),
      epsilon_explore_idx_(epsilon_explore_idx),
      replay_start_size_(replay_start_size),
      replay_memory_(replay_memory_capacity,replay_batch_size,sampleStrategy),
      update_frequency_(update_frequency),
      gamma(discount_factor),
      solver_param_(solver_param),
      evaluate(evaluate_),
      eval_epsilon(eval_epsilon_),
      current_iter_(0),
      target_q_freq(target_q_freq_),
      doubleQ(doubleQ_),
      eTrace(false),
      numSteps_(0),
      random_engine(0){}
public:
  //Initialize DeepQLearner.
  void Initialize();
  //Reset agent
  void Reset();
  //load pretrained model
  void LoadPretrainedModel(const std::string& model_file);
  //action selection - exploration and exploitation
  Action SelectAction(const InputFrames& input_frames);
  //batch q
  ActionPairVec ForwardBatchMaxQvalue(const InputFramesVec& batch_frames,const NetSp& qnet,const bool& sep);
  //network batch update 
  void MiniBatchUpdate();//using batch inputframes

public:
  //memory pool 
  MemoryReplay replay_memory_;
  //iteration idx
  int current_iteration() const { return current_iter_; }
  int numSteps() const {return numSteps_;}

private:
  void FillData2Layers(
      const FramesLayerInputData& frames_data,
      const TargetLayerInputData& target_data,
      const FilterLayerInputData& filter_data);


private:
  const ActionVect legal_actions_;
  const std::string solver_param_;
  const double gamma;
  double epsilon_start_;
  const double epsilon_min_;
  const double epsilon_decay_;
  const int epsilon_explore_idx_;
  double epsilon_;
  const int replay_start_size_;
  const int update_frequency_;
  const double eval_epsilon;
  const bool evaluate;
  const int target_q_freq;
  //hacking features
  const bool doubleQ;//enable double-Q learning
  const bool eTrace;// enbale eligibility trace



private:
  int current_iter_;
  int numSteps_;
  SolverSp solver_;
  NetSp net_;
  NetSp target_net_;//every C steps copy from net_ , C = target_q_freq
  BlobSp q_values_blob_;
  MemoryDataLayerSp frames_input_layer_;
  MemoryDataLayerSp target_input_layer_;
  MemoryDataLayerSp filter_input_layer_;
  TargetLayerInputData dummy_input_data_;

  std::mt19937 random_engine;
  
};

}

#endif /* AGENT_HPP_ */
