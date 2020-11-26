cmake_minimum_required(VERSION 3.12)
include("kae/third_party")

third_party("fmt" REPO "fmtlib/fmt" TAG "7.1.3")
system_include_dirs("fmt")
