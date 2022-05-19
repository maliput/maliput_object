#!/usr/bin/env python3
#
# Copyright 2022 Toyota Research Institute
#

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
