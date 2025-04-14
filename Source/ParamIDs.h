#pragma once
 
namespace ParamIDs
{
    // Be the same value as the "paramId" listed in the rnbo description.json.
    inline constexpr auto dry_ratio          { "dry_ratio" };
    inline constexpr auto wet_ratio          { "wet_ratio" };
    inline constexpr auto feedback_ratio     { "feedback_ratio" };
    inline constexpr auto phase_shift_mix    { "phase_shift_mix" };
    inline constexpr auto phase_shift_cutoff { "phase_shift_cutoff" };
    inline constexpr auto phase_shift_q      { "phase_shift_q" };
 
} // namespace paramIDs
