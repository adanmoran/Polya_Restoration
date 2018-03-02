//////////////////////////////////////
// Copyright MTHE 493 Group 8, 2018 //
//////////////////////////////////////

#include "matlab/MatlabProxy.h"

MatlabProxy::MatlabProxy()
{}

MatlabProxy::~MatlabProxy()
{}

auto MatlabProxy::Initialize() -> bool
{
	return false;
}

auto MatlabProxy::getSparseAdj(const std::array<size_t, 2>& rc, size_t radius, MatlabProxy::PNorm norm) -> AdjacencyMatrix
{
	AdjacencyMatrix adj(static_cast<int>(rc[0]));
	return adj;
}

auto MatlabProxy::terminate() -> void
{}

/* vim: set ts=4 sw=4 et : */