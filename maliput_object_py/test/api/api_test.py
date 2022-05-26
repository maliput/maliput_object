#!/usr/bin/env python3
#
# BSD 3-Clause License
#
# Copyright (c) 2022, Woven Planet. All rights reserved.
# Copyright (c) 2022, Toyota Research Institute. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its
#   contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""Unit tests for the maliput::api python binding"""

import unittest

from maliput_object.api import (
    BoundingRegion,
    ObjectBook,
    ObjectQuery,
    Object,
    OverlappingType,
    Id
)


class TestMaliputObjectApi(unittest.TestCase):
    """
    Evaluates the maliput_object.api bindings for concrete classes or structs.
    """

    def test_object_id(self):
        """
        Tests the RoadGeometryId binding.
        """
        dut = Id("dut")
        self.assertEqual("dut", dut.string())

    def test_overlapping_type_membes(self):
        """
        Tests that Overlapping Type members and operators.
        """
        self.assertEqual(OverlappingType(0), OverlappingType.kDisjointed)
        self.assertEqual(OverlappingType(1), OverlappingType.kIntersected)
        self.assertEqual(OverlappingType(3), OverlappingType.kContained)
        dut = OverlappingType.kContained & OverlappingType.kIntersected
        self.assertEqual(OverlappingType.kIntersected, dut)
        dut = OverlappingType.kContained | OverlappingType.kIntersected
        self.assertEqual(OverlappingType.kContained, dut)

    def test_bounding_region_methods(self):
        """
        Tests that Bounding Region exposes the right methods.
        """
        dut_type_methods = dir(BoundingRegion)
        self.assertTrue('position' in dut_type_methods)
        self.assertTrue('Contains' in dut_type_methods)
        self.assertTrue('Overlaps' in dut_type_methods)

    def test_object_book_methods(self):
        """
        Tests that Object Book exposes the right methods.
        """
        dut_type_methods = dir(ObjectBook)
        self.assertTrue('objects' in dut_type_methods)
        self.assertTrue('FindById' in dut_type_methods)
        self.assertTrue('FindByPredicate' in dut_type_methods)
        self.assertTrue('FindOverlappingIn' in dut_type_methods)

    def test_object_query_methods(self):
        """
        Tests that Object Query exposes the right methods.
        """
        dut_type_methods = dir(ObjectQuery)
        self.assertTrue('FindOverlappingLanesIn' in dut_type_methods)
        self.assertTrue('Route' in dut_type_methods)
        self.assertTrue('object_book' in dut_type_methods)
        self.assertTrue('road_network' in dut_type_methods)

    def test_object_methods(self):
        """
        Tests that Object exposes the right methods.
        """
        dut_type_methods = dir(Object)
        self.assertTrue('id' in dut_type_methods)
        self.assertTrue('bounding_region' in dut_type_methods)
        self.assertTrue('position' in dut_type_methods)
        self.assertTrue('get_properties' in dut_type_methods)
        self.assertTrue('get_property' in dut_type_methods)
