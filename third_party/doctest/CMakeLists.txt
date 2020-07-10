cmake_minimum_required(VERSION 3.12)
include("kae/third_party")

third_party("doctest" REPO "onqtam/doctest" TAG "2.3.7")
system_include_dirs("doctest")
