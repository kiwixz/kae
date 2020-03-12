cmake_minimum_required(VERSION 3.12)
include("third_party")

third_party("fmt" REPO "fmtlib/fmt" TAG "6.1.2")
system_include_dirs("fmt")
