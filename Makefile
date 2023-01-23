project := dotenv
summary := Loads environment variables from an .env file

STD := c++20

library := lib$(project)
$(library).type := shared

test.deps := $(library)
test.libs := $(project) gtest gtest_main

install.directories = $(include)/$(project)

files = $(include) $(src) Makefile VERSION

install := $(library)
targets := $(install)

include mkbuild/base.mk
