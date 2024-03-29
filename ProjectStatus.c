let
  BaseUrl = "https://api.keyedinprojects.co.uk/V3/api/report?key=687",
  GetJson = (Url) =>
    let
      RawData = Web.Contents(Url),
      Json    = Json.Document(RawData)
    in
      Json,
  GetTotalEntities = (Page) =>
    let
      Total = Page[TotalPages]
    in
      Total,
  GetPage = (Index) =>
    let
      Url  = GetUrl(Index),
      Json = GetJson(Url)
    in
      Json,
  GetUrl = (Index) =>
    let
      Url = BaseUrl & "&pageNumber=" & Text.From(Index)
    in
      Url,
  FirstPage = GetPage(1),
  EntityCount = GetTotalEntities(FirstPage),
  PageIndices = {1 .. EntityCount},
  URLs = List.Transform(PageIndices, each GetUrl(_)),
  Pages = List.Transform(PageIndices, each GetPage(_)[Data]),
  Data = List.Union(Pages),
  #"Converted to Table" = Table.FromList(
    Data,
    Splitter.SplitByNothing(),
    null,
    null,
    ExtraValues.Error
  ),
  #"Expanded Column1" = Table.ExpandRecordColumn(
    #"Converted to Table",
    "Column1",
    {
      "Project Code",
      "Project Name",
      "Project Owner",
      "Business Unit name",
      "Project Type",
      "PMO status",
      "Business Analyst Support",
      "Saving Total %",
      "Cost Avoidance %",
      "Cost Reduction %",
      "Price Increase %",
      "Service Credit/Rebate %",
      "OPEX/CAPEX TOTAL %",
      "OPEX %",
      "CAPEX %",
      "Latest Addressable CAPEX Spend (£)",
      "Latest Addressable OPEX Spend (£)",
      "Total Latest Addressable Spend (£)",
      "Annualised savings - High confidence",
      "Annualised savings - Medium confidence",
      "Annualised savings - Low confidence",
      "Waste Treatment - Operations %",
      "Waste Treatment - Delivery %",
      "Waste Network - Operations %",
      "Waste Network - Delivery %",
      "Water Production - Operations %",
      "Water Production - Delivery %",
      "Water Network - Operations %",
      "Water Network - Delivery %",
      "Customer Experience %",
      "Digital %",
      "Group Services %",
      "Operations Central %",
      "Benefit Type",
      "No. of Months",
      "FY20/21 % (option 3 only)",
      "FY21/22 % (option 3 only)",
      "FY22/23 % (option 3 only)",
      "FY23/24 % (option 3 only)",
      "FY24/25 % (option 3 only)",
      "FY25/26 % (option 3 only)",
      "Categories Box",
      "FA number (General tab)",
      "FA number (CM tab)",
      "Replacing FA number (General Tab)",
      "Replacing FA number (CM tab)",
      "Mini Competition Y/N",
      "[[CROSSTAB BREAKDOWN]]",
      "Closure",
      "Gate 2",
      "ITN Issue",
      "Gate 0",
      "Gate 1",
      "PQQ Issue"
    }
  ),
  #"Removed Columns" = Table.RemoveColumns(#"Expanded Column1", {"[[CROSSTAB BREAKDOWN]]"}),
  #"Merged Queries" = Table.NestedJoin(
    #"Removed Columns",
    {"Project Name"},
    d_Rebid,
    {"Project Name"},
    "d_Rebid",
    JoinKind.LeftOuter
  ),
  #"Expanded d_Rebid" = Table.ExpandTableColumn(
    #"Merged Queries",
    "d_Rebid",
    {"Rebid Max Term date"}
  ),
  #"Merged Queries1" = Table.NestedJoin(
    #"Expanded d_Rebid",
    {"Project Name"},
    d_sharepointFolder,
    {"Project Name"},
    "d_PulsePivot",
    JoinKind.LeftOuter
  ),
  #"Expanded d_PulsePivot" = Table.ExpandTableColumn(
    #"Merged Queries1",
    "d_PulsePivot",
    {
      "Sum of Savings - High confidence",
      "Sum of Savings - Medium confidence",
      "Sum of Savings - Low confidence"
    }
  ),
  #"Changed Type" = Table.TransformColumnTypes(
    #"Expanded d_PulsePivot",
    {
      {"Project Code", type text},
      {"Project Name", type text},
      {"Project Owner", type text},
      {"Business Unit name", type text},
      {"Project Type", type text},
      {"PMO status", type text},
      {"Business Analyst Support", type text},
      {"Saving Total %", Int64.Type},
      {"Cost Avoidance %", Int64.Type},
      {"Cost Reduction %", Int64.Type},
      {"Price Increase %", Int64.Type},
      {"Service Credit/Rebate %", type any},
      {"OPEX/CAPEX TOTAL %", Int64.Type},
      {"OPEX %", Int64.Type},
      {"CAPEX %", Int64.Type},
      {"Latest Addressable CAPEX Spend (£)", Currency.Type},
      {"Latest Addressable OPEX Spend (£)", Currency.Type},
      {"Total Latest Addressable Spend (£)", Currency.Type},
      {"Annualised savings - High confidence", Int64.Type},
      {"Annualised savings - Medium confidence", Int64.Type},
      {"Annualised savings - Low confidence", Int64.Type},
      {"Waste Treatment - Operations %", Int64.Type},
      {"Waste Treatment - Delivery %", Int64.Type},
      {"Waste Network - Operations %", Int64.Type},
      {"Waste Network - Delivery %", Int64.Type},
      {"Water Production - Operations %", Int64.Type},
      {"Water Production - Delivery %", Int64.Type},
      {"Water Network - Operations %", Int64.Type},
      {"Water Network - Delivery %", Int64.Type},
      {"Customer Experience %", Int64.Type},
      {"Digital %", Int64.Type},
      {"Group Services %", Int64.Type},
      {"Operations Central %", Int64.Type},
      {"Benefit Type", type text},
      {"No. of Months", Int64.Type},
      {"FY20/21 % (option 3 only)", Int64.Type},
      {"FY21/22 % (option 3 only)", Int64.Type},
      {"FY22/23 % (option 3 only)", Int64.Type},
      {"FY23/24 % (option 3 only)", Int64.Type},
      {"FY24/25 % (option 3 only)", Int64.Type},
      {"FY25/26 % (option 3 only)", Int64.Type},
      {"Categories Box", type text},
      {"FA number (General tab)", type text},
      {"FA number (CM tab)", type text},
      {"Replacing FA number (General Tab)", type text},
      {"Replacing FA number (CM tab)", type text},
      {"Mini Competition Y/N", type text},
      {"Closure", type date},
      {"Gate 2", type date},
      {"ITN Issue", type date},
      {"Gate 0", type date},
      {"Gate 1", type date},
      {"PQQ Issue", type date},
      {"Rebid Max Term date", type date},
      {"Sum of Savings - High confidence", Currency.Type},
      {"Sum of Savings - Medium confidence", Currency.Type},
      {"Sum of Savings - Low confidence", Currency.Type}
    }
  ),
  #"Finalising arrangement" = Table.ReorderColumns(
    #"Changed Type",
    {
      "Project Code",
      "Project Name",
      "Project Owner",
      "Business Unit name",
      "Project Type",
      "PMO status",
      "Business Analyst Support",
      "Saving Total %",
      "Cost Avoidance %",
      "Cost Reduction %",
      "Price Increase %",
      "Service Credit/Rebate %",
      "OPEX/CAPEX TOTAL %",
      "OPEX %",
      "CAPEX %",
      "Gate 0",
      "Gate 1",
      "PQQ Issue",
      "ITN Issue",
      "Gate 2",
      "Closure",
      "Latest Addressable CAPEX Spend (£)",
      "Latest Addressable OPEX Spend (£)",
      "Total Latest Addressable Spend (£)",
      "Annualised savings - High confidence",
      "Annualised savings - Medium confidence",
      "Annualised savings - Low confidence",
      "Waste Treatment - Operations %",
      "Waste Treatment - Delivery %",
      "Waste Network - Operations %",
      "Waste Network - Delivery %",
      "Water Production - Operations %",
      "Water Production - Delivery %",
      "Water Network - Operations %",
      "Water Network - Delivery %",
      "Customer Experience %",
      "Digital %",
      "Group Services %",
      "Operations Central %",
      "Benefit Type",
      "No. of Months",
      "FY20/21 % (option 3 only)",
      "FY21/22 % (option 3 only)",
      "FY22/23 % (option 3 only)",
      "FY23/24 % (option 3 only)",
      "FY24/25 % (option 3 only)",
      "FY25/26 % (option 3 only)",
      "Categories Box",
      "FA number (General tab)",
      "FA number (CM tab)",
      "Replacing FA number (General Tab)",
      "Replacing FA number (CM tab)",
      "Mini Competition Y/N",
      "Sum of Savings - High confidence",
      "Sum of Savings - Medium confidence",
      "Sum of Savings - Low confidence",
      "Rebid Max Term date"
    }
  ),
  #"Removed Columns1" = Table.RemoveColumns(
    #"Finalising arrangement",
    {
      "Saving Total %",
      "FY20/21 % (option 3 only)",
      "FY21/22 % (option 3 only)",
      "FY22/23 % (option 3 only)",
      "FY23/24 % (option 3 only)",
      "FY24/25 % (option 3 only)",
      "FY25/26 % (option 3 only)",
      "FA number (General tab)",
      "FA number (CM tab)",
      "Replacing FA number (General Tab)",
      "Replacing FA number (CM tab)",
      "Mini Competition Y/N",
      "OPEX/CAPEX TOTAL %",
      "Benefit Type",
      "No. of Months",
      "Categories Box"
    }
  ),
  #"Reordered Columns" = Table.ReorderColumns(
    #"Removed Columns1",
    {
      "Project Code",
      "Project Name",
      "Business Unit name",
      "Project Type",
      "PMO status",
      "Business Analyst Support",
      "Gate 0",
      "Gate 1",
      "PQQ Issue",
      "ITN Issue",
      "Gate 2",
      "Closure",
      "Latest Addressable CAPEX Spend (£)",
      "Latest Addressable OPEX Spend (£)",
      "Total Latest Addressable Spend (£)",
      "Annualised savings - High confidence",
      "Annualised savings - Medium confidence",
      "Annualised savings - Low confidence",
      "Waste Treatment - Operations %",
      "Waste Treatment - Delivery %",
      "Waste Network - Operations %",
      "Waste Network - Delivery %",
      "Water Production - Operations %",
      "Water Production - Delivery %",
      "Water Network - Operations %",
      "Water Network - Delivery %",
      "Customer Experience %",
      "Digital %",
      "Group Services %",
      "Operations Central %",
      "Cost Avoidance %",
      "Cost Reduction %",
      "Price Increase %",
      "Service Credit/Rebate %",
      "OPEX %",
      "CAPEX %",
      "Sum of Savings - High confidence",
      "Sum of Savings - Medium confidence",
      "Sum of Savings - Low confidence",
      "Rebid Max Term date"
    }
  ),
  #"Renamed Columns" = Table.RenameColumns(
    #"Reordered Columns",
    {
      {"Sum of Savings - High confidence", "21/22 High confidence"},
      {"Sum of Savings - Medium confidence", "21/22 Medium confidence"},
      {"Sum of Savings - Low confidence", "21/22 Low confidence"}
    }
  )
in
  #"Renamed Columns"
