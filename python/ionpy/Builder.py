import ctypes
from typing import Optional
from ctypes.util import find_library
import os

from .native import (
    c_ion_builder_t,
    c_ion_node_t,

    ion_builder_create,
    ion_builder_destroy,

    ion_builder_set_target,
    ion_builder_with_bb_module,
    ion_builder_add_node,
    ion_builder_compile,

    ion_builder_save,
    ion_builder_load,

    ion_builder_run,
)

from .Node import Node
from .BuilderCompileOption import BuilderCompileOption
from .PortMap import PortMap


class Builder:
    def __init__(self):
        c_builder = c_ion_builder_t()

        ret = ion_builder_create(ctypes.byref(c_builder))
        if ret != 0:
            raise Exception('Invalid operation')

        self.obj = c_builder

    def __del__(self):
        if self.obj: # check not nullptr
            ion_builder_destroy(self.obj)

    def set_target(self, target: str) -> 'Builder':
        ret = ion_builder_set_target(self.obj, target.encode())
        if ret != 0:
            raise Exception('Invalid operation')

        return self

    def with_bb_module(self, path: str) -> 'Builder':
        if os.name == 'nt':
            ret = ion_builder_with_bb_module(self.obj, str(find_library(path)).encode())
        else:
            ret = ion_builder_with_bb_module(self.obj, path.encode())
        if ret != 0:
            raise Exception('Invalid operation')

        return self

    def add(self, key: str) -> Node:
        c_node = c_ion_node_t()

        ret = ion_builder_add_node(self.obj, key.encode(), ctypes.byref(c_node))
        if ret != 0:
            raise Exception('Invalid operation')

        return Node(obj_=c_node)

    def compile(self, function_name: str, option: BuilderCompileOption):
        ret = ion_builder_compile(self.obj, function_name.encode(), option.to_cobj())
        if ret != 0:
            raise Exception('Invalid operation')

    def save(self, filename: str):
        ret = ion_builder_save(self.obj, filename.encode())
        if ret != 0:
            raise Exception('Invalid operation')

    def load(self, filename: str):
        ret = ion_builder_load(self.obj, filename.encode())
        if ret != 0:
            raise Exception('Invalid operation')

    def run(self, port_map: PortMap):
        ret = ion_builder_run(self.obj, port_map.obj)
        if ret != 0:
            raise Exception('Invalid operation')
