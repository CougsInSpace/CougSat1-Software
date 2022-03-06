"""/************************************************************************************************************************************************
* Programmer: Anatolii Kalinin (AK),                                                                                                               *
* Library Name:  solid_works_curves.py                                                                                                             *
*                                                                                                                                                  *
* Date: 3/3/2022 (last edited by AK)                                                                                                               *
*                                                                                                                                                  *
* Description: The purpose of this library is to communicate data between SolidWorks curve files and python                                        *
*                                                                                                                                                  *
* Relevant Information: SW - SolidWorks. SW curves are written in a .cwcur format. Each SW curve file can contain several curves, and information  *
*                            about them such as curve id, curve name. The information about each curve is stored in the curve header.              *
*                            The datapoints are stored below the header. Indentation must be added before each header and appendix of a            * 
*                            curve. Double indentation must be added before datapoint. Sample format:                                              *
*                                                                                                                                                  *
*                               <?xml version="1.0"?>                                                                                              *
*                                                                                                                                                  *
*                               <curves>                                                                                                           *
*                                                                                                                                                  *
*                                  <functioncurve id="0" name="Curve 1" type="1" source="" shape="0">                                              *
*                                     <data points= "1 0"/>                                                                                        *
*                                      <data points= "2 3"/>                                                                                       *
*                                 </functioncurve>                                                                                                 *
*                                                                                                                                                  *
*                                  <functioncurve id="1" name="Curve 2" type="1" source="" shape="0">                                              *
*                                      <data points= "1 0"/>                                                                                       *
*                                      <data points= "2 3"/>                                                                                       *
*                                 </functioncurve>                                                                                                 *
*                                                                                                                                                  *
*                               </curves>                                                                                                          *

\****************************************************************************************************************************************************/"""

import os  # used to create directories and paths


"""****************************************************************************
* Class: SW_curves     (super class)                                          *
* Date Created: 24/02/22     (AK)                                             *
* Date Last Modified: 3/3/22 (AK)                                             *
*                                                                             *
* Description: Contains information about all the curves in a family          *
*                                                                             *
* Input parameters (initialization): name (type str),                         *
*                                                                             *
* Contains: Name, number of curves, curves                                    *
*                                                                             *
* Preconditions: Desired family name                                          *
*                                                                             *
* Methods: .__init__() - initializes class                                    *
*          .add_curve() - adds a new curve with all the necessary information *
*          .generate_SW_curve_file() - generates a SW .cwcur file that        *
*               that contains all the curves in the correct format            *
*                                                                             *
*******************************************************************************
* Method: .__init__()                                                   *
* Date Created: 24/02/22     (AK)                                       *
* Date Last Modified: 3/3/22 (AK)                                       *
*                                                                       *
* Description: This method initializes SW_curves class                  *
*                                                                       *
* Input parameters: self (type SW_curves), name (type str)              *
*                                                                       *
* Returns: None                                                         *
*                                                                       *
* Preconditions: Name of family of curves is availible                  *
*                                                                       *
* Postconditions: SW_curves() instance is initialized with desired,     *
*                   parameter name set equal to the desired name,       *
*                   parameter num_curves is set to 0,                   *
*                   parameter SW_curfe is set equal to empty python list*
*************************************************************************
*************************************************************************
* Method: .add_curve()                                                  *
* Date Created: 24/02/22     (AK)                                       *
* Date Last Modified: 3/3/22 (AK)                                       *
*                                                                       *
* Description: This method appends a curve to the list of curves in the *
*               family - SW_curve                                       *
*                                                                       *
* Input parameters: self (type SW_curves), curve_name (type str), data  *
*                       (type numpy array or python list)               *
*                                                                       *
* Returns: None                                                         *
*                                                                       *
* Preconditions: SW_curve class, desired name, data, number of curves   *
                    in the class are avalible                           *
*                                                                       *
* Postconditions: Counter num_curves in the SW_curves is increased by 1,*
*                   SW_curve type is added to the end of the class list *
*                   SW_curve                                            *
*************************************************************************
*************************************************************************
* Method: .generate_SW_curve_file()                                     *
* Date Created: 24/02/22     (AK)                                       *
* Date Last Modified: 3/3/22 (AK)                                       *
*                                                                       *
* Description: This method generates a .cwcur file with all the         *
*               information about the curves availible in the class at  *
*               the moment                                              *
*                                                                       *
* Input parameters: self (type SW_curves), directory                    *
*                                                                       *
* Returns: None                                                         *
* Preconditions: SW_curve class, desired complete name with directory   *
*                   and all teh data are availibe                       *
*                                                                       *
*Postconditions: SW file .cwcur is created. The file has name stored    *
*                   in the self.name object. It contains all the        *
*                   information availible in the object at the moment   *
*************************************************************************

*******************************************************************************"""


class SW_curves:
    def __init__(self, name: str):
        self.name = name
        self.num_curves = 0
        self.SW_curve = []

    def add_curve(self, curve_name: str, data):

        self.SW_curve.append(SW_curve(self.num_curves, curve_name, data))
        self.num_curves += 1

    def generate_SW_curve_file(self, directory: str):

        header = '<?xml version="1.0"?>\n\n<curves>\n'
        appendix = "</curves>"

        SW_CWCUR_path = init_SW_CWCUR(directory, self.name)

        SW_CWCUR = open(SW_CWCUR_path, "w")

        SW_CWCUR.write(header)

        for curve in self.SW_curve:

            SW_CWCUR.write(curve.processed_contents)

        SW_CWCUR.write(appendix)

        SW_CWCUR.close()


"""****************************************************************************
* Class: SW_curves(SW_curves)     (subclass)                                  *
* Date Created: 24/02/22     (AK)                                             *
* Date Last Modified: 3/3/22 (AK)                                             *
*                                                                             *
* Description: Contains information about a curve such as its id, name, and   *
*                   data in the SW curve format                               *
*                                                                             *
* Input parameters (initialization): id (type int), name (type str),          *
*                                    data (type python list or numpy array)   *
*                                                                             *
* Contains: Id, name, and data (raw and process formats) of the curve         *
*                                                                             *
* Preconditions: Desired curve id, name, and data                             *
*                                                                             *
* Methods: .__init__() - initializes class                                    *
*                                                                             *
*******************************************************************************
* Method: .__init__()                                                   *
* Date Created: 24/02/22     (AK)                                       *
* Date Last Modified: 3/3/22 (AK)                                       *
*                                                                       *
* Description: This method initializes SW_curve class                   *
*                                                                       *
* Input parameters: self (type SW_curve), id (type int), name (type str)*
*                       data (tyoe python list, or numpy array)         *
*                                                                       *
* Returns: None                                                         *
*                                                                       *
* Preconditions: Id, name, and data of the curve are avalible           *
*                                                                       *
* Postconditions: SW_curve() class is initialized                       *
*************************************************************************"""


class SW_curve(SW_curves):
    def __init__(self, id: int, name: str, data):

        self.curve_id = id
        self.curve_name = name
        self.processed_contents = parse_to_SW_curve(id, name, data)
        self.raw_contents = data


"""************************************************************************
* Function: parse_to_SW_curve()                                         *
* Date Created: 24/02/22     (AK)                                       *
* Date Last Modified: 3/3/22 (AK)                                       *
*                                                                       *
* Description: This function creates a string in a SW curve format that *
*                contains all the necessary information about the curve *
*                                                                       *
* Input parameters: id (type int), name (type str),                     *
*                   data_stream (type numpy array orpython list)        *
*                                                                       *
* Returns: Parsed string that contains all the necessary information    *
*                   about the string                                    *
*                                                                       *
* Preconditions: Curve data, name, id of the curve is availible         *
*                                                                       *
* Postconditions: String in the necessary format with necessary data is *
*                    created                                            *
*************************************************************************"""


def parse_to_SW_curve(id: int, name: str, data_stream) -> str:

    SW_curve = ""

    header = (
        '   <functioncurve id="'
        + str(id)
        + '" name="'
        + name
        + '" type="1" source="" shape="0">\n'
    )

    appendix = "   </functioncurve>\n\n"

    SW_curve += header

    for data_point in data_stream:
        SW_curve += (
            '       <data points= "'
            + str(data_point[0])
            + " "
            + str(data_point[1])
            + '"/>\n'
        )

    SW_curve += appendix

    return SW_curve


"""************************************************************************
* Function: init_SW_CWCUR()                                             *
* Date Created: 24/02/22     (AK)                                       *
* Date Last Modified: 3/3/22 (AK)                                       *
*                                                                       *
* Description: This function initializes a .cwcur file                  *
*                                                                       *
* Input parameters: directory (type str), name (type str)               *
*                                                                       *
* Returns: Path to the initialized .cwcur file                          *
*                                                                       *
* Preconditions: os library is imported, name, and desired directory are*
*                   availible                                           *
*                                                                       *
* Postconditions: .cwcur file with a desired name in a desired directory*
*                   is created                                          *
*************************************************************************"""


def init_SW_CWCUR(directory: str, name: str) -> str:

    name = name + ".cwcur"

    completeName = os.path.join(directory, name)

    SW_file = open(completeName, "w")
    SW_file.close()

    return completeName  # returns file directory
